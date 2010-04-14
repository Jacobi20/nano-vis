using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;

namespace WCFVisualizer
{
    // NOTE: If you change the interface name "IVisualizerService" here, you must also update the reference to "IVisualizerService" in App.config.
    [ServiceContract]
    public interface IVisualizerService
    {
        [OperationContract]
        Guid uploadCubeFile(Stream inpFile);

        [OperationContract]
        Stream visualizeCubeFile(Guid fid, Dictionary<string, string> param);
    }
}
