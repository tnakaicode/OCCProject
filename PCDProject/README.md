---
title: Viewer
---

## `CreateWindowEx`

Windows API の関数で、拡張スタイルを指定してウィンドウを作成します。その引数は以下の通りです。

---

### 関数シグネチャ

```cpp
HWND CreateWindowEx(
    DWORD     dwExStyle,       // 拡張ウィンドウスタイル
    LPCWSTR   lpClassName,     // ウィンドウクラス名
    LPCWSTR   lpWindowName,    // ウィンドウのタイトル
    DWORD     dwStyle,         // ウィンドウスタイル
    int       X,               // ウィンドウの初期 x 座標
    int       Y,               // ウィンドウの初期 y 座標
    int       nWidth,          // ウィンドウの幅
    int       nHeight,         // ウィンドウの高さ
    HWND      hWndParent,      // 親ウィンドウのハンドル
    HMENU     hMenu,           // メニューのハンドル
    HINSTANCE hInstance,       // インスタンスハンドル
    LPVOID    lpParam          // ウィンドウ作成データ
);
```

---

### 引数の説明

1. **`dwExStyle`**  
   - ウィンドウの拡張スタイルを指定します。
   - 例: `WS_EX_OVERLAPPEDWINDOW`, `WS_EX_TOPMOST`, `WS_EX_TOOLWINDOW` など。

2. **`lpClassName`**  
   - ウィンドウクラス名を指定します。
   - `RegisterClass` または `RegisterClassEx` で登録したクラス名を指定します。

3. **`lpWindowName`**  
   - ウィンドウのタイトルバーに表示される文字列を指定します。

4. **`dwStyle`**  
   - ウィンドウのスタイルを指定します。
   - 例: `WS_OVERLAPPEDWINDOW`, `WS_POPUP`, `WS_CHILD` など。

5. **`X`**  
   - ウィンドウの左上隅の x 座標を指定します。
   - `CW_USEDEFAULT` を指定すると、システムがデフォルトの位置を決定します。

6. **`Y`**  
   - ウィンドウの左上隅の y 座標を指定します。
   - `CW_USEDEFAULT` を指定すると、システムがデフォルトの位置を決定します。

7. **`nWidth`**  
   - ウィンドウの幅をピクセル単位で指定します。

8. **`nHeight`**  
   - ウィンドウの高さをピクセル単位で指定します。

9. **`hWndParent`**  
   - 親ウィンドウのハンドルを指定します。
   - 親ウィンドウがない場合は `nullptr` を指定します。

10. **`hMenu`**  
    - メニューのハンドルを指定します。
    - メニューがない場合は `nullptr` を指定します。

11. **`hInstance`**  
    - アプリケーションのインスタンスハンドルを指定します。

12. **`lpParam`**  
    - ウィンドウ作成データへのポインタを指定します。
    - 通常は `nullptr` を指定します。

---

### 戻り値

- 成功時: 作成されたウィンドウのハンドル (`HWND`) を返します。
- 失敗時: `nullptr` を返します。

### 注意点

- `CreateWindowEx` を呼び出す前に、`RegisterClass` または `RegisterClassEx` を使用してウィンドウクラスを登録する必要があります。
- 作成したウィンドウを表示するには、`ShowWindow` と `UpdateWindow` を呼び出します。

これで、`CreateWindowEx` の引数について理解できるはずです。

## include

```cmd
    Directory: C:\OpenCASCADE-7.7.0-vc14-64\opencascade-7.7.0\inc


Mode                 LastWriteTime         Length Name
----                 -------------         ------ ----
-a----        2022/11/12      1:37           1053 AIS_DisplayMode.hxx
-a----        2022/11/12      1:37           1003 AIS_DisplayStatus.hxx
-a----        2022/11/12      1:37           1312 Aspect_Display.hxx
-a----        2022/11/12      1:37           6115 Aspect_DisplayConnection.hxx
-a----        2022/11/12      1:37           1582 Aspect_DisplayConnectionDefinitionError.hxx
-a----        2022/11/12      1:37           1335 Aspect_TypeOfDisplayText.hxx
-a----        2022/11/12      1:37           4368 Draw_Display.hxx
-a----        2022/11/12      1:37           1766 Graphic3d_DisplayPriority.hxx
-a----        2022/11/12      1:37           1371 IGESData_LabelDisplayEntity.hxx
-a----        2022/11/12      1:37           2130 IGESDefs_HArray1OfHArray1OfTextDisplayTemplate.hxx
-a----        2022/11/12      1:37           4921 IGESDimen_DimensionDisplayData.hxx
-a----        2022/11/12      1:37           3340 IGESDimen_ToolDimensionDisplayData.hxx
-a----        2022/11/12      1:37           4609 IGESDraw_LabelDisplay.hxx
-a----        2022/11/12      1:37           2974 IGESDraw_ToolLabelDisplay.hxx
-a----        2022/11/12      1:37           1071 IGESGraph_Array1OfTextDisplayTemplate.hxx
-a----        2022/11/12      1:37           1077 IGESGraph_HArray1OfTextDisplayTemplate.hxx
-a----        2022/11/12      1:37           4919 IGESGraph_TextDisplayTemplate.hxx
-a----        2022/11/12      1:37           3107 IGESGraph_ToolTextDisplayTemplate.hxx
-a----        2022/11/12      1:37           3434 IVtkTools_DisplayModeFilter.hxx
-a----        2022/11/12      1:37           1786 MeshVS_DisplayModeFlags.hxx
-a----        2022/11/12      1:37           1092 PrsDim_DisplaySpecialSymbol.hxx
-a----        2022/11/12      1:37           1389 PrsMgr_DisplayStatus.hxx
```

```cmd

    Directory: C:\OpenCASCADE-7.7.0-vc14-64\opencascade-7.7.0\inc


Mode                 LastWriteTime         Length Name
----                 -------------         ------ ----
-a----        2022/11/12      1:37           2354 AIS_GraphicTool.hxx
-a----        2022/11/12      1:37           1542 Aspect_GraphicDeviceDefinitionError.hxx
-a----        2022/11/12      1:37            901 Aspect_GraphicsLibrary.hxx
-a----        2022/11/12      1:37           1449 D3DHost_GraphicDriver.hxx
-a----        2022/11/12      1:37           1301 D3DHost_GraphicDriverFactory.hxx
-a----        2022/11/12      1:37           1722 Graphic3d_AlphaMode.hxx
-a----        2022/11/12      1:37           1668 Graphic3d_ArrayFlags.hxx
-a----        2022/11/12      1:37           2471 Graphic3d_ArrayOfPoints.hxx
-a----        2022/11/12      1:37           5569 Graphic3d_ArrayOfPolygons.hxx
-a----        2022/11/12      1:37           5263 Graphic3d_ArrayOfPolylines.hxx
-a----        2022/11/12      1:37          41169 Graphic3d_ArrayOfPrimitives.hxx
-a----        2022/11/12      1:37           3599 Graphic3d_ArrayOfQuadrangles.hxx
-a----        2022/11/12      1:37           4072 Graphic3d_ArrayOfQuadrangleStrips.hxx
-a----        2022/11/12      1:37           3064 Graphic3d_ArrayOfSegments.hxx
-a----        2022/11/12      1:37           3805 Graphic3d_ArrayOfTriangleFans.hxx
-a----        2022/11/12      1:37           3964 Graphic3d_ArrayOfTriangles.hxx
-a----        2022/11/12      1:37           4714 Graphic3d_ArrayOfTriangleStrips.hxx
-a----        2022/11/12      1:37           3008 Graphic3d_AspectFillArea3d.hxx
-a----        2022/11/12      1:37           2688 Graphic3d_AspectLine3d.hxx
-a----        2022/11/12      1:37           3348 Graphic3d_AspectMarker3d.hxx
-a----        2022/11/12      1:37          24269 Graphic3d_Aspects.hxx
-a----        2022/11/12      1:37           4492 Graphic3d_AspectText3d.hxx
-a----        2022/11/12      1:37           3806 Graphic3d_AttribBuffer.hxx
-a----        2022/11/12      1:37            849 Graphic3d_BndBox3d.hxx
-a----        2022/11/12      1:37            895 Graphic3d_BndBox4d.hxx
-a----        2022/11/12      1:37           1018 Graphic3d_BndBox4f.hxx
-a----        2022/11/12      1:37           2881 Graphic3d_BoundBuffer.hxx
-a----        2022/11/12      1:37           7190 Graphic3d_BSDF.hxx
-a----        2022/11/12      1:37          11318 Graphic3d_Buffer.hxx
-a----        2022/11/12      1:37           1974 Graphic3d_BufferRange.hxx
-a----        2022/11/12      1:37           1110 Graphic3d_BufferType.hxx
-a----        2022/11/12      1:37           2960 Graphic3d_BvhCStructureSet.hxx
-a----        2022/11/12      1:37           5297 Graphic3d_BvhCStructureSetTrsfPers.hxx
-a----        2022/11/12      1:37          37714 Graphic3d_Camera.hxx
-a----        2022/11/12      1:37           3243 Graphic3d_CameraTile.hxx
-a----        2022/11/12      1:37           1359 Graphic3d_CappingFlags.hxx
-a----        2022/11/12      1:37          12918 Graphic3d_CLight.hxx
-a----        2022/11/12      1:37          19117 Graphic3d_ClipPlane.hxx
-a----        2022/11/12      1:37          10017 Graphic3d_CStructure.hxx
-a----        2022/11/12      1:37           3846 Graphic3d_CubeMap.hxx
-a----        2022/11/12      1:37           6291 Graphic3d_CubeMapOrder.hxx
-a----        2022/11/12      1:37           3460 Graphic3d_CubeMapPacked.hxx
-a----        2022/11/12      1:37           2955 Graphic3d_CubeMapSeparate.hxx
-a----        2022/11/12      1:37           1223 Graphic3d_CubeMapSide.hxx
-a----        2022/11/12      1:37          12626 Graphic3d_CullingTool.hxx
-a----        2022/11/12      1:37          32545 Graphic3d_CView.hxx
-a----        2022/11/12      1:37           1784 Graphic3d_DataStructureManager.hxx
-a----        2022/11/12      1:37           1954 Graphic3d_DiagnosticInfo.hxx
-a----        2022/11/12      1:37           1766 Graphic3d_DisplayPriority.hxx
-a----        2022/11/12      1:37           7202 Graphic3d_FrameStats.hxx
-a----        2022/11/12      1:37           4459 Graphic3d_FrameStatsCounter.hxx
-a----        2022/11/12      1:37           5565 Graphic3d_FrameStatsData.hxx
-a----        2022/11/12      1:37           1155 Graphic3d_FrameStatsTimer.hxx
-a----        2022/11/12      1:37           9883 Graphic3d_GraduatedTrihedron.hxx
-a----        2022/11/12      1:37           7324 Graphic3d_GraphicDriver.hxx
-a----        2022/11/12      1:37           2537 Graphic3d_GraphicDriverFactory.hxx
-a----        2022/11/12      1:37          15578 Graphic3d_Group.hxx
-a----        2022/11/12      1:37           1248 Graphic3d_GroupAspect.hxx
-a----        2022/11/12      1:37           1518 Graphic3d_GroupDefinitionError.hxx
-a----        2022/11/12      1:37           2444 Graphic3d_HatchStyle.hxx
-a----        2022/11/12      1:37           1086 Graphic3d_HorizontalTextAlignment.hxx
-a----        2022/11/12      1:37           2718 Graphic3d_IndexBuffer.hxx
-a----        2022/11/12      1:37           8658 Graphic3d_Layer.hxx
-a----        2022/11/12      1:37           1136 Graphic3d_LevelOfTextureAnisotropy.hxx
-a----        2022/11/12      1:37           7818 Graphic3d_LightSet.hxx
-a----        2022/11/12      1:37            962 Graphic3d_MapIteratorOfMapOfStructure.hxx
-a----        2022/11/12      1:37            961 Graphic3d_MapOfAspectsToAspects.hxx
-a----        2022/11/12      1:37            976 Graphic3d_MapOfObject.hxx
-a----        2022/11/12      1:37            938 Graphic3d_MapOfStructure.hxx
-a----        2022/11/12      1:37           5618 Graphic3d_MarkerImage.hxx
-a----        2022/11/12      1:37            877 Graphic3d_Mat4.hxx
-a----        2022/11/12      1:37            871 Graphic3d_Mat4d.hxx
-a----        2022/11/12      1:37          12093 Graphic3d_MaterialAspect.hxx
-a----        2022/11/12      1:37           1548 Graphic3d_MaterialDefinitionError.hxx
-a----        2022/11/12      1:37           1856 Graphic3d_MediaTexture.hxx
-a----        2022/11/12      1:37           4547 Graphic3d_MediaTextureSet.hxx
-a----        2022/11/12      1:37           2350 Graphic3d_MutableIndexBuffer.hxx
-a----        2022/11/12      1:37           4907 Graphic3d_NameOfMaterial.hxx
-a----        2022/11/12      1:37           1001 Graphic3d_NameOfTexture1D.hxx
-a----        2022/11/12      1:37           1530 Graphic3d_NameOfTexture2D.hxx
-a----        2022/11/12      1:37           1173 Graphic3d_NameOfTextureEnv.hxx
-a----        2022/11/12      1:37           1086 Graphic3d_NameOfTexturePlane.hxx
-a----        2022/11/12      1:37           1006 Graphic3d_NMapOfTransient.hxx
-a----        2022/11/12      1:37           9733 Graphic3d_PBRMaterial.hxx
-a----        2022/11/12      1:37           1515 Graphic3d_PolygonOffset.hxx
-a----        2022/11/12      1:37           4442 Graphic3d_PresentationAttributes.hxx
-a----        2022/11/12      1:37           1548 Graphic3d_PriorityDefinitionError.hxx
-a----        2022/11/12      1:37           1101 Graphic3d_RenderingMode.hxx
-a----        2022/11/12      1:37          20389 Graphic3d_RenderingParams.hxx
-a----        2022/11/12      1:37           1299 Graphic3d_RenderTransparentMethod.hxx
-a----        2022/11/12      1:37            985 Graphic3d_SequenceOfGroup.hxx
-a----        2022/11/12      1:37           4454 Graphic3d_SequenceOfHClipPlane.hxx
-a----        2022/11/12      1:37           1007 Graphic3d_SequenceOfStructure.hxx
-a----        2022/11/12      1:37           1820 Graphic3d_ShaderAttribute.hxx
-a----        2022/11/12      1:37           2889 Graphic3d_ShaderFlags.hxx
-a----        2022/11/12      1:37          12002 Graphic3d_ShaderManager.hxx
-a----        2022/11/12      1:37           5492 Graphic3d_ShaderObject.hxx
-a----        2022/11/12      1:37          12649 Graphic3d_ShaderProgram.hxx
-a----        2022/11/12      1:37           4974 Graphic3d_ShaderVariable.hxx
-a----        2022/11/12      1:37           2505 Graphic3d_ShaderVariable.lxx
-a----        2022/11/12      1:37           1661 Graphic3d_StereoMode.hxx
-a----        2022/11/12      1:37          21614 Graphic3d_Structure.hxx
-a----        2022/11/12      1:37           1558 Graphic3d_StructureDefinitionError.hxx
-a----        2022/11/12      1:37           8393 Graphic3d_StructureManager.hxx
-a----        2022/11/12      1:37           4967 Graphic3d_Text.hxx
-a----        2022/11/12      1:37           1004 Graphic3d_TextPath.hxx
-a----        2022/11/12      1:37           2283 Graphic3d_Texture1D.hxx
-a----        2022/11/12      1:37           1750 Graphic3d_Texture1Dmanual.hxx
-a----        2022/11/12      1:37           2749 Graphic3d_Texture1Dsegment.hxx
-a----        2022/11/12      1:37           2894 Graphic3d_Texture2D.hxx
-a----        2022/11/12      1:37           1081 Graphic3d_Texture2Dmanual.hxx
-a----        2022/11/12      1:37           5178 Graphic3d_Texture2Dplane.hxx
-a----        2022/11/12      1:37           1935 Graphic3d_Texture3D.hxx
-a----        2022/11/12      1:37           2286 Graphic3d_TextureEnv.hxx
-a----        2022/11/12      1:37           2874 Graphic3d_TextureMap.hxx
-a----        2022/11/12      1:37           7248 Graphic3d_TextureParams.hxx
-a----        2022/11/12      1:37           8117 Graphic3d_TextureRoot.hxx
-a----        2022/11/12      1:37           3086 Graphic3d_TextureSet.hxx
-a----        2022/11/12      1:37           1483 Graphic3d_TextureSetBits.hxx
-a----        2022/11/12      1:37           4343 Graphic3d_TextureUnit.hxx
-a----        2022/11/12      1:37           1064 Graphic3d_ToneMappingMethod.hxx
-a----        2022/11/12      1:37          25280 Graphic3d_TransformPers.hxx
-a----        2022/11/12      1:37           2316 Graphic3d_TransformPersScaledAbove.hxx
-a----        2022/11/12      1:37          20336 Graphic3d_TransformUtils.hxx
-a----        2022/11/12      1:37           1810 Graphic3d_TransModeFlags.hxx
-a----        2022/11/12      1:37           1208 Graphic3d_TypeOfAnswer.hxx
-a----        2022/11/12      1:37           1888 Graphic3d_TypeOfBackfacingModel.hxx
-a----        2022/11/12      1:37           1124 Graphic3d_TypeOfBackground.hxx
-a----        2022/11/12      1:37           1023 Graphic3d_TypeOfConnection.hxx
-a----        2022/11/12      1:37           1945 Graphic3d_TypeOfLightSource.hxx
-a----        2022/11/12      1:37           3195 Graphic3d_TypeOfLimit.hxx
-a----        2022/11/12      1:37           1171 Graphic3d_TypeOfMaterial.hxx
-a----        2022/11/12      1:37           3474 Graphic3d_TypeOfPrimitiveArray.hxx
-a----        2022/11/12      1:37           1116 Graphic3d_TypeOfReflection.hxx
-a----        2022/11/12      1:37           1487 Graphic3d_TypeOfShaderObject.hxx
-a----        2022/11/12      1:37           3510 Graphic3d_TypeOfShadingModel.hxx
-a----        2022/11/12      1:37           1105 Graphic3d_TypeOfStructure.hxx
-a----        2022/11/12      1:37           1654 Graphic3d_TypeOfTexture.hxx
-a----        2022/11/12      1:37           1162 Graphic3d_TypeOfTextureFilter.hxx
-a----        2022/11/12      1:37           1071 Graphic3d_TypeOfTextureMode.hxx
-a----        2022/11/12      1:37           1120 Graphic3d_TypeOfVisualization.hxx
-a----        2022/11/12      1:37            947 Graphic3d_Vec.hxx
-a----        2022/11/12      1:37           1188 Graphic3d_Vec2.hxx
-a----        2022/11/12      1:37           1188 Graphic3d_Vec3.hxx
-a----        2022/11/12      1:37           1188 Graphic3d_Vec4.hxx
-a----        2022/11/12      1:37           3181 Graphic3d_Vertex.hxx
-a----        2022/11/12      1:37           1366 Graphic3d_VerticalTextAlignment.hxx
-a----        2022/11/12      1:37           2103 Graphic3d_ViewAffinity.hxx
-a----        2022/11/12      1:37           5266 Graphic3d_WorldViewProjState.hxx
-a----        2022/11/12      1:37           1732 Graphic3d_ZLayerId.hxx
-a----        2022/11/12      1:37           9958 Graphic3d_ZLayerSettings.hxx
-a----        2022/11/12      1:37           9730 OpenGl_GraphicDriver.hxx
-a----        2022/11/12      1:37           1628 OpenGl_GraphicDriverFactory.hxx
-a----        2022/11/12      1:37           2425 Vrml_OrthographicCamera.hxx
```

https://github.com/microsoft/vcpkg/issues/31901

This is target Visual Studio 17.6.1 and windows.
After using static linking mostly till now I've generated a triplet that mixes static and dynamic libraries from vcpkg
All seemed to be working and all is ok provided I build things from IDE.

However when building from the command line using msbuild in some situations which i can't track done IncrementalClean is getting triggered on the dlls that have been deployed to the folder the exe is in. This happens with incremental builds. And not always, sometimes they there, sometime they disappear.
Once this happens the only way to get them to reappear is a full rebuild. Either from the command line or the IDE.

The Solution is also generating a couple of dlls. And these end up in the dependent exe's folder, and don't get removed.
Only the dll's deployed from vcpkg are disappearing.

I'm not using the manifest mode for vcpkg. There is single global installation.

It sounds similar to:
dotnet/msbuild#1054

But I've had no success in trying to work around the problem.
Any tips on where to look appreciated.

My msbuild skills are lacking.


The Solution is also generating a couple of dlls. And these end up in the dependent exe's folder, and don't get removed.
Only the dll's deployed from vcpkg are disappearing.

I don't understand what "only dlls deployed from vcpkg are disappearing" mean? Can you provide an example of what went wrong?


My solution has a locally built dll.
The dll from that "local" project remains the exe's output folder.
But the dll's that came from teh vcpkg installation are deleted.


My solution has a locally built dll. The dll from that "local" project remains the exe's output folder. But the dll's that came from teh vcpkg installation are deleted.

Are the dll files in the vcpkg/install directory disappearing when recompiling?


I have a solution that has the following projects:

Static Lib - that uses vcpkg libs (some static & some dll)
DLL lib - that doesn't use vcpkg at all
Application that links the 2 local libs + also some vcpkg libs (some static & some dll)
If I do a clean rebuild the application output directory (where the exe ends up) also gets the dlls
from vcpkg and solution dll. Editing and building from the IDE all the dlls remain in place while either rebuilding or incremental builds.

If I do the same things from the command line using msbuild. The dlls are present only if I do a rebuild.
The solution dll is always present. But the dlls source from the vcpkg install have disappeared.
I see a log message about incrementalClean deleting the vcpkg dlls from teh exe output folder.

Opening the ide and doing a build, does NOT restore the dlls.

Doing a full rebuild of the solution from either the command line OR the ide and the vcpkg dlls will reappear in the exe folder.

The vcpkg install folder is all good.


This is also happening to me. On first build or clean rebuild, all of the vcpkg dll's are in the correct place, and the solution runs. On successive rebuilds, or during packaging into an msixbundle, I run into the build log below:

2>Target _CreateBundle:
2> Skipping target "_CreateBundle" because all output files are up-to-date with respect to the input files.
2>Target _GenerateAppxPackage:
2> Deleting file "D:\ProjectName\buildtree\arm64\Release\ProjectName_uwp_installerinfo.log".
2>Target IncrementalClean:
2> Deleting file "D:\ProjectName\buildtree\arm64\Release\ProjectName_uwp\ProjectName_uwp.vcxproj.GenerateResource.Cache".
2> Deleting file "D:\ProjectName\buildtree\output\arm64\Release\ProjectName_uwp\ProjectName_1.9.7.0_scale-100.msix".
2> Deleting file "D:\ProjectName\buildtree\output\arm64\Release\ProjectName_uwp\ProjectName_1.9.7.0_scale-125.msix".
2> Deleting file "D:\ProjectName\buildtree\output\arm64\Release\ProjectName_uwp\ProjectName_1.9.7.0_scale-150.msix".
2> Deleting file "D:\ProjectName\buildtree\output\arm64\Release\ProjectName_uwp\ProjectName_1.9.7.0_scale-400.msix".
2> Deleting file "D:\ProjectName\buildtree\output\arm64\Release\ProjectName_uwp\abseil_dll.dll".
2> Deleting file "D:\ProjectName\buildtree\output\arm64\Release\ProjectName_uwp\azure-core.dll".
2> Deleting file "D:\ProjectName\buildtree\output\arm64\Release\ProjectName_uwp\libcurl.dll".
2> Deleting file "D:\ProjectName\buildtree\output\arm64\Release\ProjectName_uwp\zlib1.dll".
2> Deleting file "D:\ProjectName\buildtree\output\arm64\Release\ProjectName_uwp\azure-storage-blobs.dll".
2> Deleting file "D:\ProjectName\buildtree\output\arm64\Release\ProjectName_uwp\azure-storage-common.dll".
2> Deleting file "D:\ProjectName\buildtree\output\arm64\Release\ProjectName_uwp\datachannel.dll".
2> Deleting file "D:\ProjectName\buildtree\output\arm64\Release\ProjectName_uwp\srtp2.dll".
2> Deleting file "D:\ProjectName\buildtree\output\arm64\Release\ProjectName_uwp\libssl-3-arm64.dll".
2> Deleting file "D:\ProjectName\buildtree\output\arm64\Release\ProjectName_uwp\libcrypto-3-arm64.dll".
2> Deleting file "D:\ProjectName\buildtree\output\arm64\Release\ProjectName_uwp\juice.dll".
2> Deleting file "D:\ProjectName\buildtree\output\arm64\Release\ProjectName_uwp\libprotobuf-lite.dll".
2> Deleting file "D:\ProjectName\buildtree\output\arm64\Release\ProjectName_uwp\libprotobuf.dll".
2> Deleting file "D:\ProjectName\buildtree\output\arm64\Release\ProjectName_uwp\re2.dll".
2> Deleting file "D:\ProjectName\buildtree\output\arm64\Release\ProjectName_uwp\cares.dll".
2> Deleting file "D:\ProjectName\buildtree\output\arm64\Release\ProjectName_uwp\jsoncpp.dll".
2>Target FinalizeBuildStatus:
2> Deleting file "D:\ProjectName\buildtree\arm64\Release\ProjectName_uwp\ProjectName.tlog\unsuccessfulbuild".
2>
2>Build succeeded.
2> 0 Warning(s)
2> 0 Error(s)
2>

All of these are dll's linked in through vcpkg, while other dll's I've specified in the linker for other non-vcpkg external libraries are left alone. All of these dll's are necessary for the project to run, obviously.


My work around has been to insert the following <target/> into my vcxproj files.
I just output a message as a reminder for the future when this breaks something else.
I arrived at this thru random attempts with no real knowledge of how msbuild works.
Need to be after the <import/>.

<?xml version="1.0" encoding="utf-8"?>
<Project DefaultTargets="Build" ToolsVersion="15.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
.....snip....
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />
  <Target Name="IncrementalClean">
    <Message Text="IncrementalClean Overridden. This may have undesired long term problems." />
  </Target>
  <ItemGroup>
....



@jimwang118 I am also experiencing this issue, and would love to resolve it. It's happening on both installs of Visual Studio I have (Version 17.3.5 and also 17.7.0 Preview 3) on two different machines (one is my laptop, another is my desktop), so it's reproducible across systems as well.

It happens whether I'm targeting either a local x86 build for the Hololens 2 Emulator, or an arm64 build for using on the Hololens 2 directly. It seems to be happening when doing a quick rebuild without source modifications. For example, in a sample workflow:

Make Source modifications
Build and Run using to debug, works great
Upon trying to "Run" the deployment again, it deletes all of the "unused" files, which in this case is the dll files which were copied in the post-source changes build. After it runs the MBBuild config, it repackages/rebundles the PackageLayout, which in this case doesn't contain the dll's anymore. Upon trying to invoke the executable on the emulator/device/local, the dll's are missing from the package (because they had been deleted).
The workaround I've had to use in order to make a build/test cycle work means that every time I want to start a debugging session, I have to make a minor source code change (even if it's just removing a blank line, saving, putting it back, saving) which triggers the MSBuild tasks to make sure to include the dll's coming from vcpkg. If I have no source modifications (in my own project), then it deletes the dll's that were previously copied from vcpkg, because it thinks it doesn't need them and they're useless artifacts.


Side note: this issue occurs with and also without having incremental builds enabled.


+1 Me and my colleagues have experienced this issue as well (Visual Studio 17.7.0). For some it's rare but for others it happens very frequently. Seems random, but I guess it's whenever the IncrementalClean is triggered.


See if this is dotnet/msbuild#9709


same problem here. using the vcpkg, even a restarting will result in the deletion of the DLL files copied by vcpkg.

two workarounds I found:

make a small change for any cpp file before starting debug.
disable vcpkg autodeploy dll and copy those dll files manually by yourself.



A workaround I found:

diff --git a/scripts/buildsystems/msbuild/vcpkg.targets b/scripts/buildsystems/msbuild/vcpkg.targets
index c831b0fce..db7152fbd 100644
--- a/scripts/buildsystems/msbuild/vcpkg.targets
+++ b/scripts/buildsystems/msbuild/vcpkg.targets
@@ -199,7 +199,7 @@
     </CreateProperty>
   </Target>

-  <Target Name="AppLocalFromInstalled" AfterTargets="CopyFilesToOutputDirectory" BeforeTargets="CopyLocalFilesOutputGroup;RegisterOutput"
+  <Target Name="AppLocalFromInstalled" AfterTargets="CopyFilesToOutputDirectory" BeforeTargets="CopyLocalFilesOutputGroup"
           Condition="'$(_ZVcpkgClassicOrManifest)' == 'true' and '$(VcpkgApplocalDeps)' == 'true' and '$(LinkSkippedExecution)' != 'true' and '@(Link)' != ''">
     <Message Text="[vcpkg] Starting VcpkgApplocalDeps" Importance="low" />
     <PropertyGroup>
I am uncertain about the consequences of this modification, but as of now, the behavior appears to be correct.



My work around has been to insert the following <target/> into my vcxproj files. I just output a message as a reminder for the future when this breaks something else. I arrived at this thru random attempts with no real knowledge of how msbuild works. Need to be after the <import/>.

<?xml version="1.0" encoding="utf-8"?>
<Project DefaultTargets="Build" ToolsVersion="15.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
.....snip....
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />
  <Target Name="IncrementalClean">
    <Message Text="IncrementalClean Overridden. This may have undesired long term problems." />
  </Target>
  <ItemGroup>
....
This fixed it for me but yeah IDK if this is the way to go. Please fix MSFT!!!

### vcpkg DLLがMSBuildのIncrementalCleanで消える問題 まとめ

---

#### 【現象】

- vcpkg経由で導入したDLL（例: zlib1.dll, libcurl.dllなど）が、**コマンドラインやMSIXパッケージ作成時のビルドで突然出力フォルダから消える**。
- ローカルでビルドしたDLLは消えないが、vcpkg由来のDLLだけが消える。
- IDEからのビルドでは発生しにくいが、コマンドラインや一部の再ビルド・パッケージング時に発生。
- 一度消えると、**フルリビルドしないとDLLが復活しない**。

---

#### 【原因】

- MSBuildの`IncrementalClean`ターゲットが「不要なファイル」と誤認し、vcpkgが自動コピーしたDLLを削除してしまう。
- vcpkgのMSBuild連携（`vcpkg.targets`）のタイミングや、MSBuildのファイル追跡仕様が根本原因。
- [dotnet/msbuild#9709](https://github.com/dotnet/msbuild/issues/9709) などMSBuild側の既知問題も関係。

---

#### 【既知の回避策・ワークアラウンド】

1. **ソースコードに小さな変更を加えてからビルドする**  
   → ファイルが「必要」と認識され、DLLが消えない。

2. **vcpkgの自動DLLコピーを無効化し、DLLを手動でコピーする**  
   → `VcpkgApplocalDeps`を無効化し、必要なDLLを自分で管理。

3. **`IncrementalClean`ターゲットをプロジェクトで上書きする**  
   `.vcxproj`の最後に以下を追加（ただし副作用に注意）:
   ````xml
   <Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />
   <Target Name="IncrementalClean">
     <Message Text="IncrementalClean Overridden. This may have undesired long term problems." />
   </Target>
   ````

4. **vcpkgの`vcpkg.targets`をパッチする**  
   [このコメント](https://github.com/microsoft/vcpkg/issues/31901#issuecomment-1854523842)のように  
   `AppLocalFromInstalled`ターゲットの`BeforeTargets`から`RegisterOutput`を外す。

---

#### 【注意点】

- どの回避策も**根本解決ではなく一時的なもの**です。
- MSBuildやvcpkgのアップデートで仕様が変わる可能性があります。
- 副作用（他のクリーン処理が効かなくなる等）に注意してください。

---

#### 【参考リンク】

- [vcpkg issue #31901](https://github.com/microsoft/vcpkg/issues/31901)
- [dotnet/msbuild#9709](https://github.com/dotnet/msbuild/issues/9709)

---

#### 【まとめ】

- **vcpkg DLLが消えるのはMSBuildのIncrementalCleanの仕様とvcpkg連携のタイミング問題が原因**
- **現状は手動コピーやターゲット上書きなどで回避するしかない**
- **根本解決にはvcpkg/MSBuild側の修正を待つ必要がある**

---

**今すぐ安定させたい場合は、vcpkg DLLの手動コピー運用が一番安全です。**
