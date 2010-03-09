using System;
using System.Collections.Generic;
using System.Text;
using nano_vis;
using System.Threading;
using System.Globalization;

namespace test1
{
	static class Program
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>

		[STAThread]
		static void Main()
		{
			Thread.CurrentThread.CurrentCulture	=	new CultureInfo("");
		
            using (NanoVis nano_vis = new NanoVis("CUBE", "test2.cube", null))
            {
				nano_vis.Run();
			}
		}
	}
}
