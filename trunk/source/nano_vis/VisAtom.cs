using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using SampleFramework;
using SlimDX;
using SlimDX.Direct3D9;
using System.Windows.Forms;
using OpenBabel;


namespace nano_vis
{
	public class VisAtom : IDisposable
	{
		Device	d3ddev;
		Effect	atom_fx;
		Effect	wire_fx;
		Effect	vol_fx;
		
		public	Matrix	matrix_view;
		public	Matrix	matrix_proj;
		public	Vector4	light_dir;
		public  Vector4 view_dir;
		public  Vector4 view_point;
		
		Mesh	stick;
		Mesh	ball;
		VolumeTexture	volume_data = null;
		Texture			palette = null;
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
			
			Mesh stick	=	Mesh.CreateCylinder(d3ddev, 1, 1, 1, 16, 1);
			Mesh ball	=	Mesh.CreateSphere(d3ddev, 1, 32, 16);

            VertexElement[] decl = new VertexElement[] {
                new VertexElement(0, 0,  DeclarationType.Float3, DeclarationMethod.Default, DeclarationUsage.Position, 0),
				new VertexElement(0, 12, DeclarationType.Float3, DeclarationMethod.Default, DeclarationUsage.Normal, 0),
				new VertexElement(0, 24, DeclarationType.Float3, DeclarationMethod.Default, DeclarationUsage.TextureCoordinate, 0),
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
			if (palette	!=	null	)	{	palette.Dispose();		}
		}

		/*---------------------------------------------------------------------
		 * 
		---------------------------------------------------------------------*/
		public void SetPalette(string path) 
		{
			if (palette!=null) palette.Dispose();
			palette = null;
			try {
				palette	=	Texture.FromFile(d3ddev, path);
			} catch (Exception ex) {
				MessageBox.Show (null, "File \""+ path + "\" not found", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
		}
		
		public void UpdateVolume( OBGridData grid )
		{
			//VectorInt vi  = grid.GetNumberOfSteps();
		
			int nx=100, ny=100, nz=100;
			//nx=	vi[0];
			//ny=	vi[1];
			//nz=	vi[2];
			
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
					
						double value = grid.GetValue(i+0, j+0, k+0);
						
						//if (i>1 && i<nx-1 &&
						//    j>1 && j<ny-1 &&
						//    k>1 && k<nz-1 ) {
						//    value += grid.GetValue(i-1, j+0, k+0);
						//    value += grid.GetValue(i+1, j+0, k+0);
						//    value += grid.GetValue(i+0, j-1, k+0);
						//    value += grid.GetValue(i+0, j+1, k+0);
						//    value += grid.GetValue(i+0, j-1, k+0);
						//    value += grid.GetValue(i+0, j+1, k+0);
						//    value/=7;
						//}
					
						int pos	=	4*i + j * box.RowPitch + k * box.SlicePitch;
						box.Data.Seek(pos, System.IO.SeekOrigin.Begin);
					
						box.Data.Write<float>( (float) value );
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

		public void SetupVolume ( Matrix w, Matrix v, Matrix p, uint slice_num, float vol_scale )
		{
			vol_fx.Technique = "volume";
			vol_fx.SetValue("matrix_world",		w );
			vol_fx.SetValue("matrix_view",		v );
			vol_fx.SetValue("matrix_proj",		p );
			vol_fx.SetValue("view_point",		view_point );
			vol_fx.SetValue("slice_num",		(float)slice_num);
			vol_fx.SetValue("vol_scale",		(float)vol_scale);
			
			Matrix	box_matrix = Matrix.Identity;
			Vector3 vvx = new Vector3((float)vx.x(), (float)vx.y(), (float)vx.z());
			Vector3 vvy = new Vector3((float)vy.x(), (float)vy.y(), (float)vy.z());
			Vector3 vvz = new Vector3((float)vz.x(), (float)vz.y(), (float)vz.z());
			box_matrix.set_Rows(0, new Vector4(50.0f*vvx, 0));	// there is a size of the grid
			box_matrix.set_Rows(1, new Vector4(50.0f*vvy, 0));
			box_matrix.set_Rows(2, new Vector4(50.0f*vvz, 0));

			//float offsetx = -0.005f; vvx
			//float offsety = -0.005f;
			//float offsetz = -0.005f;
			//box_matrix *= Matrix.Translation(offsetx, offsety, offsetz);

			vol_fx.SetValue("matrix_box",		box_matrix );
			
			vol_fx.SetTexture("volume_data_tex", volume_data);
			vol_fx.SetTexture("palette_tex", palette);
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
			public VertUV(Vector3 _p, uint _c, Vector2 _uv, Vector2 _uv1, Vector3 _n) {
				p	= _p;
				c	= _c;
				uv	= _uv;
				uv1	= _uv1;
				n	= _n;
			}
			Vector3 p;
			Vector3 n;
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

		/*---------------------------------------------------------------------
		 * Sticks :
		---------------------------------------------------------------------*/
		public struct Stick {
				public Stick ( Vector3 p, Vector3 d, Vector4 c, float l, float r) {
					position	=	p;
					direction	=	d;
					color		=	c;
					length		=	l;
					radius		=	r;
				}
				public Vector3	position;
				public Vector3	direction;
				public Vector4	color;
				public float	length;
				public float	radius;
				
			}

		public void DrawSticks(Stick[] sticks)
		{
			atom_fx.Technique = "solid_body";
			atom_fx.SetValue("matrix_view",		matrix_view );
			atom_fx.SetValue("matrix_proj",		matrix_proj );
			atom_fx.SetValue("light_dir",		Vector4.Normalize(light_dir));
			atom_fx.SetValue("view_dir",		Vector4.Normalize(view_dir));
			
			EffectHandle	h_matrix_world	=	atom_fx.GetParameter(null, "matrix_world");
			EffectHandle	h_atom_color	=	atom_fx.GetParameter(null, "atom_color");

			atom_fx.Begin();
			atom_fx.BeginPass(0);

			d3ddev.VertexDeclaration	=	new VertexDeclaration(d3ddev, stick.GetDeclaration());
			d3ddev.SetStreamSource( 0, stick.VertexBuffer, 0, 36);
			d3ddev.Indices	=	stick.IndexBuffer;
		
			for (uint i=0; i<sticks.Length; i++) {
				Stick	s	=	sticks[i];
			
				Vector3 dir		= Vector3.Normalize(s.direction);
				Vector3 up		= new Vector3(0,0,1);
				
				if (Math.Abs(Vector3.Dot(dir, up))==1) {
				    up = new Vector3(1,0,0);
				}
				
				Matrix	w	=	Matrix.Identity;
				        w	*=	Matrix.Scaling(s.radius, s.radius, s.length);
				        w	*=	Matrix.Translation(0,0, 0.5f * s.length);
				        w	*=	Matrix.Invert(Matrix.LookAtLH(s.position, s.position + dir, up));

				atom_fx.SetValue(h_matrix_world,	w );
				atom_fx.SetValue(h_atom_color,		s.color);
				
				atom_fx.CommitChanges();

				d3ddev.DrawIndexedPrimitives( PrimitiveType.TriangleList, 0, 0, stick.VertexCount, 0, stick.FaceCount);
			}
			
			h_matrix_world.Dispose();
			h_atom_color.Dispose();

			d3ddev.VertexDeclaration.Dispose();
			d3ddev.VertexDeclaration = null;

			atom_fx.EndPass();
			atom_fx.End();
		}
		
		
		/*---------------------------------------------------------------------
		 * Balls :
		---------------------------------------------------------------------*/
			
		public struct Ball {
				public Ball(Vector3 p, Vector4 c, float r) {
					pos		=	p;
					color	=	c;
					radius	=	r;
				}
				public Vector3	pos;
				public Vector4 color;
				public float	radius;
			}
			

		public void DrawBalls(Ball[] balls) 
		{
			atom_fx.Technique = "solid_body";
			atom_fx.SetValue("matrix_view",		matrix_view );
			atom_fx.SetValue("matrix_proj",		matrix_proj );
			atom_fx.SetValue("light_dir",		Vector4.Normalize(light_dir));
			atom_fx.SetValue("view_dir",		Vector4.Normalize(view_dir));
			
			EffectHandle	h_matrix_world	=	atom_fx.GetParameter(null, "matrix_world");
			EffectHandle	h_atom_color	=	atom_fx.GetParameter(null, "atom_color");

			atom_fx.Begin();
			atom_fx.BeginPass(0);

			d3ddev.VertexDeclaration	=	new VertexDeclaration(d3ddev, ball.GetDeclaration());
			d3ddev.SetStreamSource( 0, ball.VertexBuffer, 0, 36);
			d3ddev.Indices	=	ball.IndexBuffer;

			for (uint i=0; i<balls.Length; i++) {
			
				Ball b = balls[i];

				Matrix	w	=	Matrix.Identity;
				        w	*=	Matrix.Scaling(b.radius, b.radius, b.radius);
				        w	*=	Matrix.Translation(b.pos);

				atom_fx.SetValue(h_matrix_world,	w );
				atom_fx.SetValue(h_atom_color,		b.color);
				
				atom_fx.CommitChanges();

				d3ddev.DrawIndexedPrimitives( PrimitiveType.TriangleList, 0, 0, ball.VertexCount, 0, ball.FaceCount);
			}
			
			h_matrix_world.Dispose();
			h_atom_color.Dispose();

			
			d3ddev.VertexDeclaration.Dispose();
			d3ddev.VertexDeclaration = null;

			atom_fx.EndPass();
			atom_fx.End();
		}
		
		/*---------------------------------------------------------------------
		 * Drawing stuff :
		---------------------------------------------------------------------*/

		public void DrawVolume(uint steps, float vol_scale)
		{
			SetupVolume(Matrix.Identity, matrix_view, matrix_proj, steps, vol_scale);
			
			vol_fx.Begin();
			vol_fx.BeginPass(0);

			d3ddev.VertexFormat	=	VertexFormat.Position | VertexFormat.Diffuse | VertexFormat.Texture2 | VertexFormat.Normal;

			//	Z planes :
			for (uint i=0; i<steps; i++) {			
				float	z	=	((float) i / (float) steps) * 2 - 1;
				float	s	=	((float) i / (float) steps);
				VertUV[]	verts = {
					new VertUV(new Vector3(+1, +1, z), 0xFF00FF00, new Vector2(1,1), new Vector2(s,0), new Vector3(0,0,1)), 
					new VertUV(new Vector3(-1, +1, z), 0xFF000000, new Vector2(0,1), new Vector2(s,0), new Vector3(0,0,1)), 
					new VertUV(new Vector3(-1, -1, z), 0x00000000, new Vector2(0,0), new Vector2(s,0), new Vector3(0,0,1)), 
					new VertUV(new Vector3(+1, -1, z), 0x0000FF00, new Vector2(1,0), new Vector2(s,0), new Vector3(0,0,1)),
				};
				d3ddev.DrawUserPrimitives<VertUV>(PrimitiveType.TriangleFan, 2, verts);
			}			

			for (uint i=0; i<steps; i++) {			
			    float	z	=	((float) i / (float) steps) * 2 - 1;
			    float	s	=	((float) i / (float) steps);
			    VertUV[]	verts = {
			        new VertUV(new Vector3(+1, z, +1), 0xFF00FF00, new Vector2(1,s), new Vector2(1,0), new Vector3(0,1,0)), 
			        new VertUV(new Vector3(-1, z, +1), 0xFF000000, new Vector2(0,s), new Vector2(1,0), new Vector3(0,1,0)), 
			        new VertUV(new Vector3(-1, z, -1), 0x00000000, new Vector2(0,s), new Vector2(0,0), new Vector3(0,1,0)), 
			        new VertUV(new Vector3(+1, z, -1), 0x0000FF00, new Vector2(1,s), new Vector2(0,0), new Vector3(0,1,0)),
			    };
			    d3ddev.DrawUserPrimitives<VertUV>(PrimitiveType.TriangleFan, 2, verts);
			}			

			for (uint i=0; i<steps; i++) {			
			    float	z	=	((float) i / (float) steps) * 2 - 1;
			    float	s	=	((float) i / (float) steps);
			    VertUV[]	verts = {
			        new VertUV(new Vector3( z, +1, +1), 0xFF00FF00, new Vector2(s,1), new Vector2(1,0), new Vector3(1,0,0)), 
			        new VertUV(new Vector3( z, -1, +1), 0xFF000000, new Vector2(s,0), new Vector2(1,0), new Vector3(1,0,0)), 
			        new VertUV(new Vector3( z, -1, -1), 0x00000000, new Vector2(s,0), new Vector2(0,0), new Vector3(1,0,0)), 
			        new VertUV(new Vector3( z, +1, -1), 0x0000FF00, new Vector2(s,1), new Vector2(0,0), new Vector3(1,0,0)),
			    };
			    d3ddev.DrawUserPrimitives<VertUV>(PrimitiveType.TriangleFan, 2, verts);
			}			


			vol_fx.EndPass();
			vol_fx.End();
		}
		
		public void DrawVolumeSlices ( BoundingBox box )
		{
		}
		
	}
}
