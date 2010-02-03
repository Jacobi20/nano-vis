using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using SampleFramework;
using SlimDX;
using SlimDX.Direct3D9;
using OpenBabel;


namespace nano_vis
{
	abstract class VisBase
	{
		public abstract	void UpdateTraceRay	( Vector3 p0, Vector3 p1 );
		public abstract void Draw3D			( NanoVis nano_vis );
		public abstract void Draw2D			( NanoVis nano_vis );
		public abstract void ReloadData		( NanoVis nano_vis );
		
		public Color		Background	{ get; set; }
	}
}
