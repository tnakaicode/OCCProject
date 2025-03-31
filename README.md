  <ItemGroup>
    <ClCompile Include="main.cpp" />
    <ClCompile Include="ViewerWidget.cpp" />
  </ItemGroup>
  <ItemGroup>
    <ClInclude Include="ViewerWidget.h" />
  </ItemGroup>
  <ItemGroup>
    <!-- MOC対象のヘッダーファイルを指定 -->
    <CustomBuild Include="ViewerWidget.h">
      <Command>
        "C:\OpenCASCADE-7.7.0-vc14-64\qt5.11.2-vc14-64\bin\moc.exe" "%(FullPath)" -o "$(IntDir)moc_%(Filename).cpp"
      </Command>
      <Outputs>$(IntDir)moc_%(Filename).cpp</Outputs>
      <Message>Running MOC on %(Filename).h</Message>
    </CustomBuild>
  </ItemGroup>
  <ItemGroup>
    <!-- MOCで生成されたファイルをコンパイル対象に追加 -->
    <ClCompile Include="$(IntDir)moc_ViewerWidget.cpp" />
  </ItemGroup>
