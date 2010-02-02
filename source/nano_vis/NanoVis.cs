using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;
using System.Diagnostics;
using SampleFramework;
using SlimDX;
using SlimDX.Direct3D9;
using OpenBabel;

namespace nano_vis
{
	enum  OpenBabelDataType	{
	  UndefinedData = 0, PairData = 1, EnergyData = 2, CommentData = 3,
	  ConformerData = 4, ExternalBondData = 5, RotamerList = 6, VirtualBondData = 7,
	  RingData = 8, TorsionData = 9, AngleData = 10, SerialNums = 11,
	  UnitCell = 12, SpinData = 13, ChargeData = 14, SymmetryData = 15,
	  ChiralData = 16, OccupationData = 17, DensityData = 18, ElectronicData = 19,
	  VibrationData = 20, RotationData = 21, NuclearData = 22, SetData = 23,
	  GridData = 24, VectorData = 25, MatrixData = 26, CustomData0 = 16384,
	  CustomData1 = 16385, CustomData2 = 16386, CustomData3 = 16387, CustomData4 = 16388,
	  CustomData5 = 16389, CustomData6 = 16390, CustomData7 = 16391, CustomData8 = 16392,
	  CustomData9 = 16393, CustomData10 = 16394, CustomData11 = 16395, CustomData12 = 16396,
	  CustomData13 = 16397, CustomData14 = 16398, CustomData15 = 16399
	}

	class NanoVis : Game
	{
		public VisAtom	vis_atom	;
		public VisCube	vis_cube;
		
		const int InitialWidth = 800;
		const int InitialHeight = 600;
		const int ParticleCount = 1500;
		
		float	cam_aspect		=	1.0f;
		
		float	view_distance	=	30;
		Matrix	view_rotation	=	new Matrix();
		Matrix	view_offset		=	new Matrix();
		int		old_mouse_x;
		int		old_mouse_y;

		Camera	camera		= new Camera();
		
		Matrix	global_view;
		Matrix	global_proj;
		
		SlimDX.Direct3D9.Font	font;
		SlimDX.Direct3D9.Font	font_big;
		
		
		Form	form_settings;
		PropertyGrid	property_editor;
		
		//Molecule	molecule;

		public Device Device
		{
			get { return GraphicsDeviceManager.Direct3D9.Device; }
		}

		public Color ClearColor
		{
			get;
			set;
		}

		/*---------------------------------------------------------------------
		 * Init :
		---------------------------------------------------------------------*/
		public NanoVis()
		{
			Debug.WriteLine("INIT : NanoVis");
			ClearColor = Color.Black;

			view_rotation			=	Matrix.Identity;

			Window.ClientSize	=	new Size(InitialWidth, InitialHeight);
			Window.Text			=	"Nano Visualizer";
			Window.KeyDown		+=	Window_KeyDown;
			Window.MouseMove	+=	Window_MouseMove;
			Window.Width		=	800;
			Window.Height		=	600;
			Window.Top			=	100;
			Window.Left			=	100;

			camera.FieldOfView	=	(float)(Math.PI / 4);
			camera.NearPlane	=	1.0f;
			camera.FarPlane		=	1000.0f;
			camera.Location		=	new Vector3(0, 0.0f, 0.0f);
			camera.Target		=	Vector3.Zero;
			camera.Up			=	new Vector3(0,0,1);
			cam_aspect			=	(float)Window.Width / Window.Height;
			
			//	
			//	Setup device :
			//	
			DeviceSettings	dev		= new DeviceSettings();
            dev.DeviceVersion		= DeviceVersion.Direct3D9;
            dev.Windowed			= true;
            dev.BackBufferWidth		= InitialWidth;
            dev.BackBufferHeight	= InitialHeight;
            dev.MultisampleType		= MultisampleType.FourSamples;
			
			
			vis_cube	=	new VisCube("test3.cube");
			
			GraphicsDeviceManager.ChangeDevice(dev);
			//GraphicsDeviceManager.ChangeDevice(DeviceVersion.Direct3D9, true, InitialWidth, InitialHeight);
			InitSettingsWindow();
			
		}
		
		void InitSettingsWindow()
		{
			form_settings			=	new Form();
			form_settings.Text		=	"Settings";
			form_settings.Width		=	300;
			form_settings.Height	=	400;
			
			property_editor	=	new PropertyGrid();
			property_editor.Parent	=	form_settings;
			property_editor.Dock	=	DockStyle.Fill;
			
			property_editor.SelectedObject	=	vis_cube;
			
		}

		void Window_KeyDown(object sender, KeyEventArgs e)
		{
			// F1 toggles between full screen and windowed mode
			// Escape quits the application
			if (e.KeyCode == Keys.F1) {
				GraphicsDeviceManager.ToggleFullScreen();
			}
			if (e.KeyCode == Keys.F2) {
				if (form_settings.IsDisposed) {
					InitSettingsWindow();
				} 
				if (!form_settings.Visible) {
					form_settings.Show(Window);
				}
			}
			else if (e.KeyCode == Keys.Escape) {
				Exit();
			}
		}
		
		void Window_MouseMove(object sender, MouseEventArgs e) 
		{
			int dx = e.X - old_mouse_x;
			int dy = e.Y - old_mouse_y;
			
			float fx = -1 + 2 * (e.X / (float)Window.ClientSize.Width);
			float fy =  1 - 2 * (e.Y / (float)Window.ClientSize.Height);

			Vector4	pp0		= new Vector4(fx, fy, 0, 1);
			Vector4	pp1		= new Vector4(fx, fy, 1, 1);
			Matrix	unproj	= global_view * global_proj;
			unproj.Invert();
			Vector4	p0		= Vector4.Transform(pp0, unproj);
			Vector4	p1		= Vector4.Transform(pp1, unproj);
			p0 /= p0.W;
			p1 /= p1.W;
			
			vis_cube.UpdateTraceRay(new Vector3(p0.X, p0.Y, p0.Z), new Vector3(p1.X, p1.Y, p1.Z));
		
			if (e.Button==MouseButtons.Left) {
				view_rotation	*=	Matrix.RotationY(0.01f * dx);
				view_rotation	*=	Matrix.RotationX(0.01f * dy);
				//angle_z	+= 0.01f*dx;
				//angle_y	+= 0.01f*dy;
			}
			if (e.Button==MouseButtons.Right) {
				view_distance *= (float)Math.Pow(1.01f, (float)dy);
			}
		
			old_mouse_x	=	e.X;
			old_mouse_y	=	e.Y;
		}
		
		public void DrawText ( int x, int y, string text )
		{
			font.DrawString(null, text, x,y, new Color4(1,1,1,1));
		}

		public void DrawTextBig ( int x, int y, string text )
		{
			font_big.DrawString(null, text, x,y, new Color4(1,1,1,1));
		}

		/*---------------------------------------------------------------------
		 * Loading :
		---------------------------------------------------------------------*/
		protected override void LoadContent()
		{
			Debug.WriteLine("Loading content...");
			cam_aspect	=	(float)Window.Width / Window.Height;
			vis_atom	=	new VisAtom(Device);
			font		=	new SlimDX.Direct3D9.Font(Device, new System.Drawing.Font(FontFamily.GenericSansSerif, 10));
			font_big	=	new SlimDX.Direct3D9.Font(Device, new System.Drawing.Font(FontFamily.GenericSansSerif, 20));
			
			vis_cube.ReloadData( this );
		}

		/*---------------------------------------------------------------------
		 * Unoading :
		---------------------------------------------------------------------*/
		protected override void UnloadContent()
		{
			Debug.WriteLine("Unloading content...");
			
			vis_atom.Dispose();
			font.Dispose();
		}

		/*---------------------------------------------------------------------
		 * Rendering :
		---------------------------------------------------------------------*/
		protected override void Draw(GameTime gameTime)
		{
			Matrix	view	=	Matrix.Identity;
					view	*=	view_offset;
					view	*=	view_rotation;
					view	*=	Matrix.Translation(0, 0, -view_distance);
			Matrix	proj	=	Matrix.PerspectiveFovRH(3.14f/6.0f, cam_aspect, 0.1f, 1000.0f);
			
			ClearColor	=	vis_cube.Background;
			
			try {
		
				Device.Clear(ClearFlags.Target | ClearFlags.ZBuffer, ClearColor, 1.0f, 0);
				Device.BeginScene();

					DrawParticles(view, proj);

					vis_cube.Draw2D(this);

				Device.EndScene();
				
			
			} catch (Exception ex) {
				Debug.WriteLine("**** EXCEPTION ****");
				Debug.WriteLine(ex.Message);
			}
		}

		void DrawParticles(Matrix matrix_view, Matrix matrix_proj)
		{
			try {
				global_view				=	matrix_view;
				global_proj				=	matrix_proj;
				
				vis_atom.matrix_proj	=	matrix_proj;
				vis_atom.matrix_view	=	matrix_view;
				vis_atom.light_dir		=	new Vector4(0,0,1,0);
				vis_atom.view_dir		=	Vector4.Transform( new Vector4(0,0,1,0), Matrix.Invert(view_rotation));

				Vector3 mol_center = new Vector3(0,0,0);		

				view_offset	=	Matrix.Translation( -mol_center );
				//vis_atom.DrawAxis( mol_center );
				
				vis_cube.Draw3D(this);
				
			} catch (Exception ex) {
				Debug.WriteLine("ERROR : " + ex.Message);
			}					
		}
	}
}
