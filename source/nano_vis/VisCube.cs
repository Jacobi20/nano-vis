using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Drawing;
using SampleFramework;
using SlimDX;
using SlimDX.Direct3D9;
using System.Windows.Forms;
using OpenBabel;

namespace nano_vis
{
	class VisCube : VisBase
	{
		OBMol			mol;
		OBConversion	conv;
		OBGridData		grid;
		OBElementTable	elem_table;

		bool	palette_changed	=	true;
		string	palette_path	=	"palette.tga";
		
		public float		VolumeScale	{ get; set; }
		public Color		BondColor	{ get; set; }
		public float		BondRadius	{ get; set; }
		public float		AtomScale	{ get; set; }
		public bool			UseVDWRadius{ get; set; }
		public bool			ShowDensity	{ get; set; }
		public bool			ShowAtoms	{ get; set; }
		public uint			SliceNum	{ get; set; }
		public string		palette		{ get { return palette_path; } set { palette_path = value; palette_changed = true; } }
		
		Vector3	trace_p0;
		Vector3	trace_p1;
		bool	trace_updated;
		OBAtom	atom_under_cursor = null;
		bool	right_click;
		

		public override	bool		HasAnimation	(  ) { return false; }
		public override void		SetTime			( float current ) {}
		public override	void		GetTiming		( out float start, out float end, out float current ) {
			start = end = current = 0;
		}

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
			ShowDensity		=	true;
			SliceNum		=	100;
			ShowAtoms		=	true;
			VolumeScale		=	100;
		
			//
			//	Load molecule :
			//	
			elem_table	=	new OBElementTable();
			conv		=	new OBConversion();
			mol			=	new OBMol();
			
			if (!conv.SetInFormat("CUBE")) {
			    throw new Exception("cann`t set input format");
			}
			
			if (!conv.ReadFile(mol, filepath)) {
			    throw new Exception("cann`t open \""+filepath+"\" file");
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
			}		
		}
		
		/*---------------------------------------------------------------------
		 * Update Trace
		---------------------------------------------------------------------*/
		override public void UpdateTraceRay( Vector3 p0, Vector3 p1 )
		{
			trace_p0			=	p0;
			trace_p1			=	p1;
			trace_updated		=	true;
		}

		public override void Focus( NanoVis nano_vis )
		{
			if (atom_under_cursor==null) {
				return;
			}
			Vector3 pos;
			pos.X = (float)atom_under_cursor.GetX();
			pos.Y = (float)atom_under_cursor.GetY();
			pos.Z = (float)atom_under_cursor.GetZ();
		
			nano_vis.Move( pos );
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
		
		override public void ReloadData(NanoVis nano_vis)
		{
			nano_vis.vis_atom.UpdateVolume( grid );
		}

		/*---------------------------------------------------------------------
		 * Draw3D
		 * draws 3d stuff :
		---------------------------------------------------------------------*/
		override public void Draw2D (NanoVis nano_vis)
		{
			if (ShowDensity) {
				nano_vis.vis_atom.DrawVolume(Math.Max(SliceNum,1), VolumeScale);
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
		VisAtom.Ball	GetBallFromAtom(OBAtom a) {
			VectorDouble	color   = elem_table.GetRGB((int)a.GetAtomicNum());
			Vector3 pos;
			Vector4 clr;
			pos.X = (float)a.GetX();
			pos.Y = (float)a.GetY();
			pos.Z = (float)a.GetZ();
			
			clr.X = (float)color[0];
			clr.Y = (float)color[1];
			clr.Z = (float)color[2];
			clr.W = 1;

			float vdwr = 1;
			if (UseVDWRadius) {				
				vdwr = (float)elem_table.GetVdwRad( (int)a.GetAtomicNum() );
			}
			float radius = AtomScale * vdwr;
			
			return new VisAtom.Ball(pos, clr, radius);
		}
		
		
		override public void Draw3D (NanoVis nano_vis)
		{
			if (palette_changed) {
				palette_changed	=	false;
				nano_vis.vis_atom.SetPalette( palette );
			}
		
		
			VisAtom	vis_atom	 = nano_vis.vis_atom;
			OBMolAtomIter	a_it = new OBMolAtomIter(mol);
			OBMolBondIter	b_it = new OBMolBondIter(mol);
			
			float time		=	1;
			
			OBAtom	old_atom_under_cursor = atom_under_cursor;
			if (trace_updated) {
				atom_under_cursor = null;
			}
			
			VisAtom.Ball[]	balls = new VisAtom.Ball[mol.NumAtoms()];
			
			uint i=0;
			for (;; i++) {
			
				if (!a_it.MoveNext()) {
					break;
				}
				OBAtom	a = a_it.Current;
				
				VisAtom.Ball	ball	=	GetBallFromAtom(a);

				if (trace_updated) {
				    float t;
				    if (TraceAgainstSphere(ball.pos, ball.radius, trace_p0, trace_p1, out t)) {
				        if (t<time) {
				            time = t;
				            atom_under_cursor = a;
				        }
				    }
				}
				
				balls[i] = ball;
			}	
			
			if (ShowAtoms) {
				nano_vis.vis_atom.DrawBalls(balls);
			}

			if (atom_under_cursor!=null) {
				VisAtom.Ball ball = GetBallFromAtom(atom_under_cursor);
				float r = ball.radius;
				vis_atom.DrawCage(ball.pos, new Vector3(2*r, 2*r, 2*r), 0.5f*r, new Vector4(1,1,1,1));
			}
				

			
			VisAtom.Stick[]	sticks = new VisAtom.Stick[mol.NumBonds()];
			
			i	=	0;		
			for (;;i++) {
				if (!b_it.MoveNext()) {
					break;
				}
				
				OBBond	b	= b_it.Current;
				OBAtom	a1	= b.GetBeginAtom();
				OBAtom	a2	= b.GetEndAtom();
				
				Vector3 p1 = new Vector3((float)a1.GetX(), (float)a1.GetY(), (float)a1.GetZ());
				Vector3 p2 = new Vector3((float)a2.GetX(), (float)a2.GetY(), (float)a2.GetZ());
				float len = (p1-p2).Length();
				
				//if (ShowAtoms) {
				//    vis_atom.DrawStick(p1, p2-p1, len, BondRadius, new Vector4(BondColor.R/256.0f, BondColor.G/256.0f, BondColor.B/256.0f, 1) );
				//}
				sticks[i]	=	new VisAtom.Stick(p1, p2-p1, new Vector4(BondColor.R/256.0f, BondColor.G/256.0f, BondColor.B/256.0f, 1), len, BondRadius );
			}
			
			if (ShowAtoms) {
				nano_vis.vis_atom.DrawSticks( sticks );
			}
		}
		
		
		
	}
}
