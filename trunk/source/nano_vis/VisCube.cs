using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Drawing;
using SampleFramework;
using SlimDX;
using SlimDX.Direct3D9;
using OpenBabel;

namespace nano_vis
{
	class VisCube
	{
		OBMol			mol;
		OBConversion	conv;
		OBGridData		grid;
		OBElementTable	elem_table;
		
		public Color		Background	{ get; set; }
		public Color		BondColor	{ get; set; }
		public float		BondRadius	{ get; set; }
		public float		AtomScale	{ get; set; }
		public bool			UseVDWRadius{ get; set; }
		public bool			ShowDensity	{ get; set; }
		
		Vector3	trace_p0;
		Vector3	trace_p1;
		bool	trace_updated;
		OBAtom	atom_under_cursor = null;

		/*---------------------------------------------------------------------
		 * ctor :
		---------------------------------------------------------------------*/
		public VisCube( string filepath )
		{
			Background		=	Color.Black;
			AtomScale		=	0.20f;
			BondRadius		=	0.10f;
			BondColor		=	Color.White;
			UseVDWRadius	=	true;
		
			//
			//	Load molecule :
			//	
			elem_table	=	new OBElementTable();
			conv		=	new OBConversion();
			mol			=	new OBMol();
			
			if (!conv.SetInFormat("CUBE")) {
			    throw new Exception("cann`t set input format");
			}
			
			if (!conv.ReadFile(mol, "test3.cube")) {
			    throw new Exception("cann`t open \"test.cube\" file");
			}
						
			mol.FindRingAtomsAndBonds();
			mol.Center();
			
			VectorpData    data_set = mol.GetAllData(openbabelcsharp.GridData);

			for (int i=0; i<data_set.Count; i++) {
				Debug.WriteLine("Reading data set entry : " + i.ToString());
				OBGenericData data = data_set[i];
				grid = data.Downcast<OBGridData>();  // <-- exception
				Debug.WriteLine("MaxValue :   " + grid.GetMaxValue());
				Debug.WriteLine("MinValue :   " + grid.GetMinValue());
				Debug.WriteLine("Attribute :  " + grid.GetAttribute());
				Debug.WriteLine("Points num : " + grid.GetNumberOfPoints());
				OBVector3	v;
			}		
		}
		
		/*---------------------------------------------------------------------
		 * Update Trace
		---------------------------------------------------------------------*/
		public void UpdateTraceRay( Vector3 p0, Vector3 p1 )
		{
			trace_p0		=	p0;
			trace_p1		=	p1;
			trace_updated	=	true;
		}
		
		bool TraceAgainstSphere( Vector3 center, float radius, Vector3 begin, Vector3 end, out float time )
		{
			Vector3	d	=	end - begin;
			Vector3	p0	=	begin;
			Vector3 pc	=	center;
			float	r	=	radius;
			float	a	=	Vector3.Dot(d, d);
			float	b	=	2 * Vector3.Dot(d, p0 - pc);
			float	c	=	Vector3.Dot(p0-pc, p0-pc) - r*r;
			
			float	D	=	b*b - 4*a*c;
			
			if (D<0) {
				time	=	1;
				return false;
			}
			if (a==0) {
				time	=	1;
				return false;
			}
			
			float t1 = (-b + (float)Math.Sqrt(D)) / 2.0f / a;
			float t2 = (-b - (float)Math.Sqrt(D)) / 2.0f / a;

			time	=	Math.Min(t1, t2);			
			
			return true;
		}
		
		public void ReloadData(NanoVis nano_vis)
		{
			nano_vis.vis_atom.UpdateVolume( grid );
		}

		/*---------------------------------------------------------------------
		 * Draw3D
		 * draws 3d stuff :
		---------------------------------------------------------------------*/
		public void Draw2D (NanoVis nano_vis)
		{
			if (ShowDensity) {
				for (float s=-1; s<=1; s+=0.01f) {
					nano_vis.vis_atom.DrawVolume(s);
				}
			}
		
			if (atom_under_cursor==null) {
				return;
			}
			OBAtom	a	= atom_under_cursor;
			int	an		= (int)a.GetAtomicNum();
			
			nano_vis.DrawTextBig( 10, 10, elem_table.GetName(an));
			
			int h = 40;
			nano_vis.DrawText( 10, h+=15, "atomic mass: " + a.GetAtomicMass().ToString());
			nano_vis.DrawText( 10, h+=15, "atomic num: " + a.GetAtomicNum().ToString());
		}
			
		/*---------------------------------------------------------------------
		 * Draw3D
		 * draws 3d stuff :
		---------------------------------------------------------------------*/
		public void Draw3D (NanoVis nano_vis)
		{
			VisAtom	vis_atom	 = nano_vis.vis_atom;
			OBMolAtomIter	a_it = new OBMolAtomIter(mol);
			OBMolBondIter	b_it = new OBMolBondIter(mol);
			
			float time		=	1;
									
			for (;;) {
			
				if (!a_it.MoveNext()) {
					break;
				}
				OBAtom	a = a_it.Current;
			    
				CDoubleArray	coord	= a.GetCoordinate();
				VectorDouble	color   = elem_table.GetRGB((int)a.GetAtomicNum());
				Vector3 pos;
				Vector4 clr;
				pos.X = (float)coord.getitem(0);
				pos.Y = (float)coord.getitem(1);
				pos.Z = (float)coord.getitem(2);
				
				clr.X = (float)color[0];
				clr.Y = (float)color[1];
				clr.Z = (float)color[2];
				clr.W = 1;

				float vdwr = 1;
				if (UseVDWRadius) {				
					vdwr = (float)elem_table.GetVdwRad( (int)a.GetAtomicNum() );
				}
				float radius = AtomScale * vdwr;

				if (trace_updated) {
					float t;
					if (TraceAgainstSphere(pos, radius, trace_p0, trace_p1, out t)) {
						if (t<time) {
							time = t;
							atom_under_cursor = a;
						}
					}
				}
				
				if (atom_under_cursor==a) {
					vis_atom.DrawCage(pos, new Vector3(2*radius, 2*radius, 2*radius), 0.5f*radius, new Vector4(1,1,1,1));
				}
				
				vis_atom.DrawBall(pos, radius, clr);
			}	
			
			uint i=0;
			for (;;i++) {
				if (!b_it.MoveNext()) {
					break;
				}
				
				OBBond	b = b_it.Current;
				CDoubleArray c1 = b.GetBeginAtom().GetCoordinate();
				CDoubleArray c2 = b.GetEndAtom().GetCoordinate();
				Vector3 p1 = new Vector3((float)c1.getitem(0), (float)c1.getitem(1), (float)c1.getitem(2));
				Vector3 p2 = new Vector3((float)c2.getitem(0), (float)c2.getitem(1), (float)c2.getitem(2));
				float len = (p1-p2).Length();
				
				vis_atom.DrawStick(p1, p2-p1, len, BondRadius, new Vector4(BondColor.R/256.0f, BondColor.G/256.0f, BondColor.B/256.0f, 1) );
			}
		}
		
		
		
	}
}
