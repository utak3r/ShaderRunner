call generate_VisualStudio.cmd

set ROK=%date:~0,4%
set MMDD=%date:~5,2%%date:~8,2%
set HHMM=%time:~0,2%%time:~3,2%
set HHMM=%HHMM: =0%

mkdir %~dp0\build_logs
set BUILD_LOG=%~dp0\build_logs\build_ShaderRunner_log-%ROK%%MMDD%-%HHMM%.txt

MSBuild /property:Configuration=Release /target:Clean;Build /m build-VisualStudio\INSTALL.vcxproj /l:FileLogger,Microsoft.Build.Engine;logfile=%BUILD_LOG%;append=true;verbosity=normal;encoding=utf-8 /v:normal
