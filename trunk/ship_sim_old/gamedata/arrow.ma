//Maya ASCII 2008 scene
//Name: arrow.ma
//Last modified: Fri, Oct 22, 2010 01:42:39 PM
//Codeset: 1251
requires maya "2008";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya Unlimited 2008";
fileInfo "version" "2008";
fileInfo "cutIdentifier" "200708022245-704165";
fileInfo "osv" "Microsoft Windows Vista Service Pack 1 (Build 6001)\n";
createNode transform -n "pCube73";
	setAttr ".t" -type "double3" 0 0 1 ;
	setAttr ".r" -type "double3" 0 0 -20.000000000000128 ;
	setAttr ".s" -type "double3" 0.999999999999999 0.999999999999999 1 ;
createNode mesh -n "pCubeShape73" -p "pCube73";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 20 ".uvst[0].uvsp[0:19]" -type "float2" 0.375 0 0.625 0 
		0.375 0.25 0.625 0.25 0.375 0.5 0.625 0.5 0.375 0.75 0.625 0.75 0.375 1 0.625 1 0.875 
		0 0.875 0.25 0.125 0 0.125 0.25 0.625 0.125 0.875 0.125 0.875 0.25 0.625 0.25 0.625 
		0 0.875 0;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 14 ".pt[0:13]" -type "float3"  40 1.7763568e-015 0 47 0 
		0 40 1.7763568e-015 0 47 0 0 40 1.7763568e-015 0 47 0 0 40 1.7763568e-015 0 47 0 
		0 47 0 0 47 0 0 47 0 0 47 0 0 47 0 0 47 0 0;
	setAttr -s 14 ".vt[0:13]"  -3 -1 0.5 0.5 -1 0.5 -3 1 0.5 0.5 1 0.5 
		-3 1 -0.5 0.5 1 -0.5 -3 -1 -0.5 0.5 -1 -0.5 4.5000005 0 -0.5 4.5000005 0 0.5 0.49999994 
		2 0.5 0.49999994 2 -0.5 0.49999994 -2 -0.5 0.49999994 -2 0.5;
	setAttr -s 21 ".ed[0:20]"  0 1 0 2 3 0 
		4 5 0 6 7 0 0 2 0 2 4 0 
		3 5 0 4 6 0 6 0 0 7 1 0 
		8 9 0 3 10 0 5 11 0 10 11 0 
		10 9 0 11 8 0 7 12 0 1 13 0 
		12 13 0 12 8 0 13 9 0;
	setAttr -s 9 ".fc[0:8]" -type "polyFaces" 
		f 4 1 6 -3 -6 
		mu 0 4 2 3 5 4 
		f 4 3 9 -1 -9 
		mu 0 4 6 7 9 8 
		f 4 8 4 5 7 
		mu 0 4 12 0 2 13 
		f 4 -19 19 10 -21 
		mu 0 4 18 19 15 14 
		f 4 -14 14 -11 -16 
		mu 0 4 16 17 14 15 
		f 4 -7 11 13 -13 
		mu 0 4 11 3 17 16 
		f 7 -15 -12 -2 -5 0 17 20 
		mu 0 7 14 17 3 2 0 1 18 
		f 4 -10 16 18 -18 
		mu 0 4 1 10 19 18 
		f 7 -20 -17 -4 -8 2 12 15 
		mu 0 7 15 19 7 6 4 11 16 ;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
createNode materialInfo -n "materialInfo3";
createNode shadingEngine -n "lambert4SG";
	setAttr ".ihi" 0;
	setAttr -s 5 ".dsm";
	setAttr ".ro" yes;
createNode lambert -n "green_signal";
	setAttr ".c" -type "float3" 0 1 0 ;
	setAttr ".it" -type "float3" 0.3137255 0.3137255 0.3137255 ;
	setAttr ".ic" -type "float3" 0 0.3137255 0 ;
createNode lightLinker -n "lightLinker1";
	setAttr -s 31 ".lnk";
	setAttr -s 31 ".slnk";
select -ne :time1;
	setAttr ".o" 13;
select -ne :renderPartition;
	setAttr -s 31 ".st";
select -ne :renderGlobalsList1;
select -ne :defaultShaderList1;
	setAttr -s 7 ".s";
select -ne :postProcessList1;
	setAttr -s 2 ".p";
select -ne :defaultRenderUtilityList1;
	setAttr -s 2 ".u";
select -ne :lightList1;
select -ne :defaultTextureList1;
select -ne :initialShadingGroup;
	setAttr ".ro" yes;
select -ne :initialParticleSE;
	setAttr ".ro" yes;
select -ne :defaultRenderQuality;
	setAttr ".eaa" 1;
select -ne :defaultResolution;
	setAttr ".w" 1344;
	setAttr ".h" 768;
	setAttr ".pa" 1;
	setAttr ".dar" 1.75;
select -ne :hardwareRenderGlobals;
	setAttr ".ctrs" 256;
	setAttr ".btrs" 512;
select -ne :defaultHardwareRenderGlobals;
	setAttr ".fn" -type "string" "im";
	setAttr ".res" -type "string" "ntsc_4d 646 485 1.333";
connectAttr "lambert4SG.msg" "materialInfo3.sg";
connectAttr "green_signal.msg" "materialInfo3.m";
connectAttr "green_signal.oc" "lambert4SG.ss";
connectAttr "pCubeShape71.iog" "lambert4SG.dsm" -na;
connectAttr "pCubeShape72.iog" "lambert4SG.dsm" -na;
connectAttr "pCubeShape73.iog" "lambert4SG.dsm" -na;
connectAttr "pCubeShape74.iog" "lambert4SG.dsm" -na;
connectAttr "pCubeShape75.iog" "lambert4SG.dsm" -na;
connectAttr ":defaultLightSet.msg" "lightLinker1.lnk[0].llnk";
connectAttr ":initialShadingGroup.msg" "lightLinker1.lnk[0].olnk";
connectAttr ":defaultLightSet.msg" "lightLinker1.lnk[1].llnk";
connectAttr ":initialParticleSE.msg" "lightLinker1.lnk[1].olnk";
connectAttr ":defaultLightSet.msg" "lightLinker1.lnk[27].llnk";
connectAttr "lambert4SG.msg" "lightLinker1.lnk[27].olnk";
connectAttr ":defaultLightSet.msg" "lightLinker1.slnk[0].sllk";
connectAttr ":initialShadingGroup.msg" "lightLinker1.slnk[0].solk";
connectAttr ":defaultLightSet.msg" "lightLinker1.slnk[1].sllk";
connectAttr ":initialParticleSE.msg" "lightLinker1.slnk[1].solk";
connectAttr ":defaultLightSet.msg" "lightLinker1.slnk[27].sllk";
connectAttr "lambert4SG.msg" "lightLinker1.slnk[27].solk";
connectAttr "lambert4SG.pa" ":renderPartition.st" -na;
connectAttr "green_signal.msg" ":defaultShaderList1.s" -na;
connectAttr "lightLinker1.msg" ":lightList1.ln" -na;
// End of arrow.ma
