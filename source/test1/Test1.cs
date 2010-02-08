﻿using System;
using System.Collections.Generic;
using System.Text;
using nano_vis;

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
			//Thread.CurrentThread.CurrentCulture	=	new CultureInfo("");
		
            using (NanoVis nano_vis = new NanoVis("CUBE", "test3.cube"))
            {
				nano_vis.Run();
			}
		}
	}
}
