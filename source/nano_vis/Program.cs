using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using System.Threading;
using System.Globalization;

namespace nano_vis
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
		
            using (NanoVis nano_vis = new NanoVis())
            {
				nano_vis.Run();
			}
		}
	}
}
