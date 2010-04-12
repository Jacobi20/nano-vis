using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.IO;
using System.Threading;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Collections.Specialized;
using System.ServiceModel;
using System.ServiceModel.Channels;
using System.Web.Services.Protocols;


namespace nvis_svc_http
{
	class Program
	{
		[DllImport("nvis_run.dll")]
		public unsafe static extern void NVisInit();

		[DllImport("nvis_run.dll")]
		public unsafe static extern void NVisShutdown();

		[DllImport("nvis_run.dll")]
		public unsafe static extern void NVisCommand( byte[] cmd );
		
		/*---------------------------------------------------------------------
		 *	LOGF()
		---------------------------------------------------------------------*/
		public static void LOGF(string s) {
			Console.WriteLine("SVC: " + s);
		}

		/*---------------------------------------------------------------------
		 *	Main()
		---------------------------------------------------------------------*/
		static void Main(string[] args)
		{
			NVisInit();
			while (true) {
				RunHTTPListener(new string[] { "http://*:80/" });
			}
			NVisShutdown();
		}
		
		
		/*---------------------------------------------------------------------
		 *	MakeShot()
		---------------------------------------------------------------------*/
		static void MakeShot(string param_list)
		{
			string command = "NVisSnapshot( { " + param_list + " } );";
			System.Text.ASCIIEncoding  encoding=new System.Text.ASCIIEncoding();
			NVisCommand( encoding.GetBytes(command) );
		}
		
		/*---------------------------------------------------------------------
		 *	RunHTTPListener()
		---------------------------------------------------------------------*/
		public static void RunHTTPListener(string[] prefixes)
		{
			if (!HttpListener.IsSupported) {
				Console.WriteLine ("Windows XP SP2 or Server 2003 is required to use the HttpListener class.");
				return;
			}
			
			// URI prefixes are required,
			// for example "http://contoso.com:8080/index/".
			if (prefixes == null || prefixes.Length == 0) {
				throw new ArgumentException("prefixes");
			}

			// Create a listener.
			HttpListener listener = new HttpListener();
			
			// Add the prefixes.
			foreach (string s in prefixes) {
				listener.Prefixes.Add(s);
			}
			
			//
			//	start server :
			//
			listener.Start();
			
				LOGF("Listening: ");
		
				// Note: The GetContext method blocks while waiting for a request. 
				HttpListenerContext context = listener.GetContext();
				HttpListenerRequest request = context.Request;
				// Obtain a response object.
				HttpListenerResponse response = context.Response;
				
				try {
					
					//	parse query :
					LOGF("Query:");
					string param_list = "";
					for (uint i=0; i<request.QueryString.AllKeys.Length; i++) {
						LOGF("[" + i + "] : " + request.QueryString.AllKeys[i] + " = " + request.QueryString.Get((int)i));
						param_list += request.QueryString.AllKeys[i];
						param_list += " = ";
						param_list += "\"";
						param_list += request.QueryString.Get((int)i);
						param_list += "\";";
					}
					
					//	get 'data_id' :
					string data_id = request.QueryString.Get("data_id");

					//	create directory for downloaded files :
					const string dir_name = "download";
					if (!Directory.Exists(dir_name)) {
						Directory.CreateDirectory(dir_name);
					}
					
					//	create path :
					string path = dir_name + "/" + data_id + ".cube";
					
					//	if file does not exist - read it from data storage :
					if (!File.Exists(path))	{
						byte [] data = GetBinaryContent(data_id);
						File.WriteAllBytes(path, data);
					}
					
					//	set ["path"] :
					param_list += "path = " + "\"" + path + "\";";
		
					//	run visualizer :					
					MakeShot(param_list);
					
					//	construct a response.
					byte[] buffer = File.ReadAllBytes("shot.png");

					response.ContentLength64 = buffer.Length;
					System.IO.BinaryWriter output = new BinaryWriter(response.OutputStream);
					output.Write(buffer, 0, buffer.Length);
					output.Close();

					Thread.Sleep(100);
					response.ContentType	=	"image/png"	;
					response.Close();
					
				} catch (Exception ex) {
					string responseString = "<HTML><BODY>" + ex.Message + "</BODY></HTML>";
					byte[] buffer = System.Text.Encoding.UTF8.GetBytes(responseString);

					response.ContentLength64 = buffer.Length;
					System.IO.Stream output = response.OutputStream;
					output.Write(buffer, 0, buffer.Length);
					// You must close the output stream.
					output.Close();
					listener.Stop();
					
					return;
				}
				
			listener.Stop();
		}
	
		/*---------------------------------------------------------------------
		 * Storage stuff :
		---------------------------------------------------------------------*/

		public static byte[] GetBinaryContent(string storageId)
		{
			byte[] bytes = null;

			string url = GetServiceUrl("DataTransfer") + "?data_id=" + storageId;

			WebRequest req = HttpWebRequest.Create(url);
			WebResponse resp = req.GetResponse();
			Stream respStream = resp.GetResponseStream();
			var reader = new System.IO.BinaryReader(respStream);

			try	{
				//long len = respStream.Length; // не работает, вроде
				long len = GetFileSize(storageId);
				bytes = reader.ReadBytes((int)len);
			}
			finally	{
				reader.Close();
				respStream.Close();
				resp.Close();
			}

			return bytes;
		}

		public static string GetServiceUrl(string serviceName)
		{
			var entryPointService = new EntryPointService.EntryPointService();
			entryPointService.Url = System.Web.Configuration.WebConfigurationManager.AppSettings["EntryPointUrl"];

			return entryPointService.GetServiceUri(serviceName);
		}

		public static long GetFileSize(string storageId)
		{
			var mtomBind = new MtomMessageEncodingBindingElement();
			mtomBind.MessageVersion = MessageVersion.Soap11;
			var httpBind = new HttpTransportBindingElement();
			var binding = new CustomBinding(new BindingElement[] { mtomBind, httpBind });

			string ioServiceUrl = GetServiceUrl("Data");
			var address = new EndpointAddress(ioServiceUrl);

			var service = new StorageService.NanoIOSystemAPIInterfaceClient(binding, address);

			var getLenReq = new StorageService.getDataLengthRequest();
			getLenReq.DataId = storageId;
			long len = service.getDataLength(getLenReq).Length;
			return len;
		}
	}

}
