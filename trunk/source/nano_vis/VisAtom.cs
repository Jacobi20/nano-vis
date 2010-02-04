using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using SampleFramework;
using SlimDX;
using SlimDX.Direct3D9;
using OpenBabel;


namespace nano_vis
{
	class VisAtom : IDisposable
	{
		Device	d3ddev;
		Effect	atom_fx;
		Effect	wire_fx;
		Effect	vol_fx;
		
		public	Matrix	matrix_view;
		public	Matrix	matrix_proj;
		public	Vector4	light_dir;
		public  Vector4 view_dir;
		
		Mesh	stick;
		Mesh	ball;
		VolumeTexture	volume_data = null;
		OBVector3	vx;
		OBVector3	vy;
		OBVector3	vz;
	
		/*---------------------------------------------------------------------
		 * VisAtom
		---------------------------------------------------------------------*/
		public VisAtom(Device d3ddev)
		{	
			this.d3ddev	=	d3ddev;
			string error = "";
			Debug.WriteLine("Compiling shaders...");
			
			try {
				atom_fx =	Effect.FromFile(d3ddev, "atom.fx",		null, null, null, ShaderFlags.None, null, out error);
				wire_fx =	Effect.FromFile(d3ddev, "wire.fx",		null, null, null, ShaderFlags.None, null, out error);
				vol_fx	=	Effect.FromFile(d3ddev, "volume.fx",	null, null, null, ShaderFlags.None, null, out error);
			} catch (Exception ex) {
				Debug.WriteLine(ex.Message);
				Debug.WriteLine(error);
				throw;
			}

			Debug.WriteLine("Done.");
			
			Mesh stick	=	Mesh.CreateCylinder(d3ddev, 1, 1, 1, 32, 1);
			Mesh ball	=	Mesh.CreateSphere(d3ddev, 1, 32, 16);

            VertexElement[] decl = new VertexElement[] {
                new VertexElement(0, 0,  DeclarationType.Float3, DeclarationMethod.Default, DeclarationUsage.Position, 0),
				new VertexElement(0, 12, DeclarationType.Float3, DeclarationMethod.Default, DeclarationUsage.Normal, 0),
				new VertexElement(0, 24, DeclarationType.Float3, DeclarationMethod.Default, DeclarationUsage.TextureCoordinate, 0),
				new VertexElement(0, 36, DeclarationType.Float4, DeclarationMethod.Default, DeclarationUsage.Color, 0),
				VertexElement.VertexDeclarationEnd
            };
			
			this.stick	= stick.Clone(d3ddev, 0, decl);
			this.ball	= ball.Clone(d3ddev, 0, decl);
			
			stick.Dispose();
			ball.Dispose();
			
			//for (int i=0; i<decl.Length; i++) {
			//    Debug.WriteLine( decl[i].ToString() );
			//}
		}
		
		public void Dispose()
		{
			if (atom_fx	!=	null	)	{	atom_fx.Dispose();		}
			if (wire_fx	!=	null	)	{	wire_fx.Dispose();		}
			if (vol_fx	!=	null	)	{	vol_fx.Dispose();		}
			if (stick	!=	null	)	{	stick.Dispose();		}
			if (ball	!=	null	)	{	ball.Dispose();			}
			if (volume_data!=null	)	{	volume_data.Dispose();	}			
		}

		/*---------------------------------------------------------------------
		 * 
		---------------------------------------------------------------------*/
		public void UpdateVolume( OBGridData grid )
		{
			int nx=100, ny=100, nz=100;
			
			vx	=	grid.GetXAxis();
			vy	=	grid.GetYAxis();
			vz	=	grid.GetZAxis();
			
			Debug.WriteLine(vx.x().ToString());
			Debug.WriteLine(vx.y().ToString());
			Debug.WriteLine(vx.z().ToString());
			Debug.WriteLine(vy.x().ToString());
			Debug.WriteLine(vy.y().ToString());
			Debug.WriteLine(vy.z().ToString());
			Debug.WriteLine(vz.x().ToString());
			Debug.WriteLine(vz.y().ToString());
			Debug.WriteLine(vz.z().ToString());
			
			//volume_data	=	new VolumeTexture(d3ddev, nx, ny, nz, 1, Usage.None, Format.L8, Pool.Managed);
			if (volume_data!=null) {
				volume_data.Dispose();
			}
			volume_data	=	new VolumeTexture(d3ddev, nx, ny, nz, 1, Usage.None, Format.R32F, Pool.Managed);
			
			DataBox	box = volume_data.LockBox(0, LockFlags.None);

			for (int i=0; i<nx; i++) {
				for (int j=0; j<ny; j++) {
					for (int k=0; k<nz; k++) {
					
						int pos	=	4*i + j * box.RowPitch + k * box.SlicePitch;
						box.Data.Seek(pos, System.IO.SeekOrigin.Begin);
					
						box.Data.Write<float>( (float) grid.GetValue(i,j,k)  );
					}
				}
			}		
			
				

			volume_data.UnlockBox(0);
		}
		
		/*---------------------------------------------------------------------
		 * Setup stuff :
		---------------------------------------------------------------------*/

		Matrix InvTranspose(Matrix m) {
			Matrix w = Matrix.Transpose( Matrix.Invert( m ) );
			w.M14 = 0;
			w.M24 = 0;
			w.M34 = 0;
			w.M44 = 0;
			w.M41 = 0;
			w.M42 = 0;
			w.M43 = 0;
			w.M44 = 0;
			return w;
		}
		
		public void SetupSolid ( Matrix w, Matrix v, Matrix p, Vector4 lightdir, Vector4 viewdir, Vector4 color )
		{
			atom_fx.Technique = "solid_body";
			atom_fx.SetValue("matrix_world",	w );
			atom_fx.SetValue("matrix_view",		v );
			atom_fx.SetValue("matrix_proj",		p );
			atom_fx.SetValue("matrix_w_it",		InvTranspose( w ) );

			atom_fx.SetValue("matrix_wv_it",	InvTranspose( w * v ) );
			atom_fx.SetValue("light_dir",		Vector4.Normalize(lightdir));
			atom_fx.SetValue("view_dir",		Vector4.Normalize(viewdir));
			atom_fx.SetValue("atom_color",		color);
		}		

		public void SetupAxis ( Matrix w, Matrix v, Matrix p )
		{
			wire_fx.Technique = "axis";
			wire_fx.SetValue("matrix_world",	w );
			wire_fx.SetValue("matrix_view",		v );
			wire_fx.SetValue("matrix_proj",		p );
		}

		public void SetupVolume ( Matrix w, Matrix v, Matrix p )
		{
			vol_fx.Technique = "volume";
			vol_fx.SetValue("matrix_world",		w );
			vol_fx.SetValue("matrix_view",		v );
			vol_fx.SetValue("matrix_proj",		p );
			
			vol_fx.SetTexture("volume_data_tex", volume_data);
		}

		/*---------------------------------------------------------------------
		 * Drawing stuff :
		---------------------------------------------------------------------*/

		protected struct Vert {
			public Vert(Vector3 _p, uint _c) {
				p = _p;
				c = _c;
			}
			Vector3 p;
			uint	c;
		};
		
		protected struct VertUV {
			public VertUV(Vector3 _p, uint _c, Vector2 _uv, Vector2 _uv1) {
				p	= _p;
				c	= _c;
				uv	= _uv;
				uv1	= _uv1;
			}
			Vector3 p;
			uint	c;
			Vector2 uv;
			Vector2 uv1;
		};
		
		public void DrawAxis(Vector3 pos)
		{
			SetupAxis(Matrix.Identity, matrix_view, matrix_proj);

		    wire_fx.Begin();
		    wire_fx.BeginPass(0);
		    
				Vert[] verts = { 
					new Vert(new Vector3(0,0,0)+pos, 0xFFFF0000),  new Vert(new Vector3(1,0,0)+pos, 0xFFFF0000), 
					new Vert(new Vector3(0,0,0)+pos, 0xFF00FF00),  new Vert(new Vector3(0,1,0)+pos, 0xFF00FF00), 
					new Vert(new Vector3(0,0,0)+pos, 0xFF0000FF),  new Vert(new Vector3(0,0,1)+pos, 0xFF0000FF), 
				};
				
				d3ddev.VertexFormat	=	VertexFormat.Position | VertexFormat.Diffuse;
				d3ddev.DrawUserPrimitives<Vert>(PrimitiveType.LineList, 3, verts);

		    wire_fx.EndPass();
		    wire_fx.End();
		}
		
		
		/*---------------------------------------------------------------------
		 * Drawing stuff :
		---------------------------------------------------------------------*/

		public void DrawVolume(float slice)
		{
			SetupVolume(Matrix.Identity, matrix_view, matrix_proj);
			
			vol_fx.Begin();
			vol_fx.BeginPass(0);
			
//			float BA = 0.529177249f;
			float BA = 50.0f;
			
			float z	=	slice * (float)vz.z() * BA;
			float w =   slice / 2.0f + 0.5f;
			
				VertUV[]	verts = {
					new VertUV(new Vector3(+1*(float)vx.x()*BA, +1*(float)vy.y()*BA, z), 0xFF00FF00, new Vector2(1,1), new Vector2(w,0)), 
					new VertUV(new Vector3(-1*(float)vx.x()*BA, +1*(float)vy.y()*BA, z), 0xFF000000, new Vector2(0,1), new Vector2(w,0)), 
					new VertUV(new Vector3(-1*(float)vx.x()*BA, -1*(float)vy.y()*BA, z), 0x00000000, new Vector2(0,0), new Vector2(w,0)), 
					new VertUV(new Vector3(+1*(float)vx.x()*BA, -1*(float)vy.y()*BA, z), 0x0000FF00, new Vector2(1,0), new Vector2(w,0)),
				};
			
				d3ddev.VertexFormat	=	VertexFormat.Position | VertexFormat.Diffuse | VertexFormat.Texture2;
				d3ddev.DrawUserPrimitives<VertUV>(PrimitiveType.TriangleFan, 2, verts);
			
			vol_fx.EndPass();
			vol_fx.End();
		}
		
		
		/*---------------------------------------------------------------------
		 * Drawing stuff :
		---------------------------------------------------------------------*/

		public void DrawCage(Vector3 center, Vector3 xyz_size, float tail_size, Vector4 color) 
		{
			SetupAxis(Matrix.Identity, matrix_view, matrix_proj);

		    wire_fx.Begin();
		    wire_fx.BeginPass(0);
		    
		    uint icolor	= 0xFFFFFFFF;
		    float x		= 0.5f*xyz_size.X;
		    float y		= 0.5f*xyz_size.Y;
		    float z		= 0.5f*xyz_size.Z;
		    float s		= tail_size;
		    Vector3	p	= center;
		    
				Vert[] verts = { 
					new Vert(new Vector3( x, y, z) + p, icolor),  new Vert(new Vector3( x-s, y-0, z-0) + p, icolor), 
					new Vert(new Vector3( x, y, z) + p, icolor),  new Vert(new Vector3( x-0, y-s, z-0) + p, icolor), 
					new Vert(new Vector3( x, y, z) + p, icolor),  new Vert(new Vector3( x-0, y-0, z-s) + p, icolor), 

					new Vert(new Vector3(-x, y, z) + p, icolor),  new Vert(new Vector3(-x+s, y-0, z-0) + p, icolor), 
					new Vert(new Vector3(-x, y, z) + p, icolor),  new Vert(new Vector3(-x+0, y-s, z-0) + p, icolor), 
					new Vert(new Vector3(-x, y, z) + p, icolor),  new Vert(new Vector3(-x+0, y-0, z-s) + p, icolor), 


					new Vert(new Vector3( x,-y, z) + p, icolor),  new Vert(new Vector3( x-s,-y+0, z-0) + p, icolor), 
					new Vert(new Vector3( x,-y, z) + p, icolor),  new Vert(new Vector3( x-0,-y+s, z-0) + p, icolor), 
					new Vert(new Vector3( x,-y, z) + p, icolor),  new Vert(new Vector3( x-0,-y+0, z-s) + p, icolor), 

					new Vert(new Vector3(-x,-y, z) + p, icolor),  new Vert(new Vector3(-x+s,-y+0, z-0) + p, icolor), 
					new Vert(new Vector3(-x,-y, z) + p, icolor),  new Vert(new Vector3(-x+0,-y+s, z-0) + p, icolor), 
					new Vert(new Vector3(-x,-y, z) + p, icolor),  new Vert(new Vector3(-x+0,-y+0, z-s) + p, icolor), 



					new Vert(new Vector3( x, y,-z) + p, icolor),  new Vert(new Vector3( x-s, y-0,-z+0) + p, icolor), 
					new Vert(new Vector3( x, y,-z) + p, icolor),  new Vert(new Vector3( x-0, y-s,-z+0) + p, icolor), 
					new Vert(new Vector3( x, y,-z) + p, icolor),  new Vert(new Vector3( x-0, y-0,-z+s) + p, icolor), 

					new Vert(new Vector3(-x, y,-z) + p, icolor),  new Vert(new Vector3(-x+s, y-0,-z+0) + p, icolor), 
					new Vert(new Vector3(-x, y,-z) + p, icolor),  new Vert(new Vector3(-x+0, y-s,-z+0) + p, icolor), 
					new Vert(new Vector3(-x, y,-z) + p, icolor),  new Vert(new Vector3(-x+0, y-0,-z+s) + p, icolor), 


					new Vert(new Vector3( x,-y,-z) + p, icolor),  new Vert(new Vector3( x-s,-y+0,-z+0) + p, icolor), 
					new Vert(new Vector3( x,-y,-z) + p, icolor),  new Vert(new Vector3( x-0,-y+s,-z+0) + p, icolor), 
					new Vert(new Vector3( x,-y,-z) + p, icolor),  new Vert(new Vector3( x-0,-y+0,-z+s) + p, icolor), 

					new Vert(new Vector3(-x,-y,-z) + p, icolor),  new Vert(new Vector3(-x+s,-y+0,-z+0) + p, icolor), 
					new Vert(new Vector3(-x,-y,-z) + p, icolor),  new Vert(new Vector3(-x+0,-y+s,-z+0) + p, icolor), 
					new Vert(new Vector3(-x,-y,-z) + p, icolor),  new Vert(new Vector3(-x+0,-y+0,-z+s) + p, icolor), 
				};
				
				d3ddev.VertexFormat	=	VertexFormat.Position | VertexFormat.Diffuse;
				d3ddev.DrawUserPrimitives<Vert>(PrimitiveType.LineList, 24, verts);

		    wire_fx.EndPass();
		    wire_fx.End();
		}

		public void DrawStick(Vector3 position, Vector3 direction, float length, float radius, Vector4 color)
		{
			Vector3 dir		= Vector3.Normalize(direction);
			Vector3 up		= new Vector3(0,0,1);
			
			if (Math.Abs(Vector3.Dot(dir, up))==1) {
				up = new Vector3(1,0,0);
			}
			
			Matrix	w	=	Matrix.Identity;
					w	*=	Matrix.Scaling(radius, radius, length);
					w	*=	Matrix.Translation(0,0, 0.5f * length);
					w	*=	Matrix.Invert(Matrix.LookAtLH(position, position + dir, up));

			SetupSolid( w, matrix_view, matrix_proj, light_dir, view_dir, color );

		    atom_fx.Begin();
		    atom_fx.BeginPass(0);

				stick.DrawSubset(0);

		    atom_fx.EndPass();
		    atom_fx.End();
		}
		
		
		public void DrawBall(Vector3 position, float radius, Vector4 color)
		{
			Matrix	w	=	Matrix.Identity;
					w	*=	Matrix.Scaling(radius, radius, radius);
					w	*=	Matrix.Translation(position);

			SetupSolid( w, matrix_view, matrix_proj, light_dir, view_dir, color );

			atom_fx.Begin();
			atom_fx.BeginPass(0);

				ball.DrawSubset(0);

			atom_fx.EndPass();
			atom_fx.End();
		}
		
	}
}
