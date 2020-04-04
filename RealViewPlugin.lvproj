<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="11008008">
	<Item Name="My Computer" Type="My Computer">
		<Property Name="NI.SortType" Type="Int">3</Property>
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="RealView Plugin" Type="Folder">
			<Item Name="Debug" Type="Folder">
				<Item Name="InstrSim" Type="Folder">
					<Item Name="write.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/Debug/InstrSim/Serial/write.vi"/>
					<Item Name="close.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/Debug/InstrSim/Serial/close.vi"/>
					<Item Name="open.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/Debug/InstrSim/Serial/open.vi"/>
					<Item Name="read.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/Debug/InstrSim/Serial/read.vi"/>
					<Item Name="remaining_string.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/Debug/InstrSim/Serial/remaining_string.vi"/>
				</Item>
				<Item Name="Map" Type="Folder">
					<Item Name="EMB_Debug_Map_RV_UpdateDD.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/Debug/Map/EMB_Debug_Map_RV_UpdateDD.vi"/>
					<Item Name="EMB_Debug_Map_RV_Fixed.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/Debug/Map/EMB_Debug_Map_RV_Fixed.vi"/>
					<Item Name="EMB_Debug_Map_RV_ResolveSymbols.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/Debug/Map/EMB_Debug_Map_RV_ResolveSymbols.vi"/>
				</Item>
			</Item>
			<Item Name="PSW" Type="Folder">
				<Item Name="PSW_Close.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/uVision/DebugMonitor/PSW_Close.vi"/>
				<Item Name="PSW_Globals.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/uVision/DebugMonitor/PSW_Globals.vi"/>
				<Item Name="PSW_Globals_Icon_Select.ctl" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/uVision/DebugMonitor/PSW_Globals_Icon_Select.ctl"/>
				<Item Name="PSW_Globals_Operation.ctl" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/uVision/DebugMonitor/PSW_Globals_Operation.ctl"/>
				<Item Name="PSW_Globals_Wrapper.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/uVision/DebugMonitor/PSW_Globals_Wrapper.vi"/>
				<Item Name="PSW_Icon_Loader.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/uVision/DebugMonitor/PSW_Icon_Loader.vi"/>
				<Item Name="PSW_Icons.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/uVision/DebugMonitor/PSW_Icons.vi"/>
				<Item Name="PSW_Path.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/uVision/DebugMonitor/PSW_Path.vi"/>
				<Item Name="PSW_Status_Disable.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/uVision/DebugMonitor/PSW_Status_Disable.vi"/>
				<Item Name="PSW_Status_Window.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/uVision/DebugMonitor/PSW_Status_Window.vi"/>
				<Item Name="PSW_Update_Application.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/uVision/DebugMonitor/PSW_Update_Application.vi"/>
				<Item Name="PSW_Update_Poller.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/uVision/DebugMonitor/PSW_Update_Poller.vi"/>
				<Item Name="PSW_Update_Processor_Status.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/uVision/DebugMonitor/PSW_Update_Processor_Status.vi"/>
				<Item Name="PSW_Update_uVision_&amp;_Processor_Status.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/uVision/DebugMonitor/PSW_Update_uVision_&amp;_Processor_Status.vi"/>
				<Item Name="PSW_Update_uVision_Status.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/uVision/DebugMonitor/PSW_Update_uVision_Status.vi"/>
				<Item Name="PSW_Update_Exception_Status.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/uVision/DebugMonitor/PSW_Update_Exception_Status.vi"/>
				<Item Name="PSW_Update_Location_Status.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/uVision/DebugMonitor/PSW_Update_Location_Status.vi"/>
				<Item Name="PSW_Poll_Sleep.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/uVision/DebugMonitor/PSW_Poll_Sleep.vi"/>
			</Item>
			<Item Name="LEP_RV_Abort.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_Abort.vi"/>
			<Item Name="LEP_RV_UVAddOptions.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_UVAddOptions.vi"/>
			<Item Name="LEP_RV_Build.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_Build.vi"/>
			<Item Name="LEP_RV_BuildSettingsDefault.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_BuildSettingsDefault.vi"/>
			<Item Name="LEP_RV_BusyBox.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_BusyBox.vi"/>
			<Item Name="LEP_RV_BusyBoxGlobal.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_BusyBoxGlobal.vi"/>
			<Item Name="LEP_RV_CCodeGenSettingsDlg.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_CCodeGenSettingsDlg.vi"/>
			<Item Name="LEP_RV_CGen.ctl" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_CGen.ctl"/>
			<Item Name="LEP_RV_CGen.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_CGen.vi"/>
			<Item Name="LEP_RV_Close.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_Close.vi"/>
			<Item Name="LEP_RV_CompilerFlags.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_CompilerFlags.vi"/>
			<Item Name="LEP_RV_CompileSettingsDlg.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_CompileSettingsDlg.vi"/>
			<Item Name="LEP_RV_Config.ctl" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_Config.ctl"/>
			<Item Name="LEP_RV_ConfigureUVProject.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_ConfigureUVProject.vi"/>
			<Item Name="LEP_RV_CreateBusyBox.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_CreateBusyBox.vi"/>
			<Item Name="LEP_RV_CreateOpBox.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_CreateOpBox.vi"/>
			<Item Name="LEP_RV_CreateUVProject.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_CreateUVProject.vi"/>
			<Item Name="LEP_RV_Debug.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_Debug.vi"/>
			<Item Name="LEP_RV_DestroyBusyBox.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_DestroyBusyBox.vi"/>
			<Item Name="LEP_RV_DestroyOpBox.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_DestroyOpBox.vi"/>
			<Item Name="LEP_RV_Download.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_Download.vi"/>
			<Item Name="LEP_RV_Error.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_Error.vi"/>
			<Item Name="LEP_RV_Errors.ctl" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_Errors.ctl"/>
			<Item Name="LEP_RV_Explore.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_Explore.vi"/>
			<Item Name="LEP_RV_GenerateBoardHeader.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_GenerateBoardHeader.vi"/>
			<Item Name="LEP_RV_GetTargetName.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_GetTargetName.vi"/>
			<Item Name="LEP_RV_InfoFile.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_InfoFile.vi"/>
			<Item Name="LEP_RV_LinkerFlags.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_LinkerFlags.vi"/>
			<Item Name="LEP_RV_LoadBuildConfig.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_LoadBuildConfig.vi"/>
			<Item Name="LEP_RV_LoadProjectInfo.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_LoadProjectInfo.vi"/>
			<Item Name="LEP_RV_MemorySettingsDlg.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_MemorySettingsDlg.vi"/>
			<Item Name="LEP_RV_MsgBox2Buttons.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_MsgBox2Buttons.vi"/>
			<Item Name="LEP_RV_MsgBox.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_MsgBox.vi"/>
			<Item Name="LEP_RV_OCDIOptionsDlg.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_OCDIOptionsDlg.vi"/>
			<Item Name="LEP_RV_OpBox.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_OpBox.vi"/>
			<Item Name="LEP_RV_OpBoxGlobal.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_OpBoxGlobal.vi"/>
			<Item Name="LEP_RV_Processor_Status.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_Processor_Status.vi"/>
			<Item Name="LEP_RV_Run.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_Run.vi"/>
			<Item Name="LEP_RV_Run_Debug.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_Run_Debug.vi"/>
			<Item Name="LEP_RV_SetupBuildFolder.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_SetupBuildFolder.vi"/>
			<Item Name="LEP_RV_Test.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_Test.vi"/>
			<Item Name="LEP_RV_UVAddFiles.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_UVAddFiles.vi"/>
			<Item Name="LEP_RV_Show_UV.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_Show_UV.vi"/>
			<Item Name="LEP_RV_MemoryMap.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_MemoryMap.vi"/>
			<Item Name="LEP_RV_MemoryMap_Cmp.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_MemoryMap_Cmp.vi"/>
			<Item Name="LEP_RV_MemoryMap_Default.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_MemoryMap_Default.vi"/>
			<Item Name="LEP_RV_MemoryMap_GetMemConfig.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_MemoryMap_GetMemConfig.vi"/>
			<Item Name="LEP_RV_MemoryMap_Query.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_MemoryMap_Query.vi"/>
			<Item Name="LEP_RV_MemoryMap_SectionNames.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_MemoryMap_SectionNames.vi"/>
			<Item Name="LEP_RV_MemoryMap_Sections.ctl" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_MemoryMap_Sections.ctl"/>
			<Item Name="LEP_RV_MemoryMap_GetAvailable.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_MemoryMap_GetAvailable.vi"/>
			<Item Name="LEP_RV_MemoryMap_SetDefaults.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_MemoryMap_SetDefaults.vi"/>
			<Item Name="LEP_RV_TargetTemplateVersion.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_TargetTemplateVersion.vi"/>
			<Item Name="LEP_RV_TCPIPSettingsDlg.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_TCPIPSettingsDlg.vi"/>
		</Item>
		<Item Name="UVControl" Type="Folder">
			<Item Name="UVControl_CopyTemplate.vi" Type="VI" URL="../common/UVControl/UVControl_CopyTemplate.vi"/>
			<Item Name="UVControl_GetLVPaths.vi" Type="VI" URL="../common/UVControl/UVControl_GetLVPaths.vi"/>
			<Item Name="UVControl_LoadLVRuntimeConfig.vi" Type="VI" URL="../common/UVControl/UVControl_LoadLVRuntimeConfig.vi"/>
			<Item Name="UVControl_LoadRealViewConfig.vi" Type="VI" URL="../common/UVControl/UVControl_LoadRealViewConfig.vi"/>
			<Item Name="UVControl_LoadTargetConfig.vi" Type="VI" URL="../common/UVControl/UVControl_LoadTargetConfig.vi"/>
			<Item Name="UVControl_Start.vi" Type="VI" URL="../common/UVControl/UVControl_Start.vi"/>
			<Item Name="UVControl_Stop.vi" Type="VI" URL="../common/UVControl/UVControl_Stop.vi"/>
			<Item Name="UVControl_UpdateGroup.vi" Type="VI" URL="../common/UVControl/UVControl_UpdateGroup.vi"/>
			<Item Name="RealViewConfig.ctl" Type="VI" URL="../common/UVControl/RealViewConfig.ctl"/>
			<Item Name="UVControl_CopyLVRuntime.vi" Type="VI" URL="../common/UVControl/UVControl_CopyLVRuntime.vi"/>
			<Item Name="UVControl_CheckLicense.vi" Type="VI" URL="../common/UVControl/UVControl_CheckLicense.vi"/>
			<Item Name="UVControl_ConvertPaths.vi" Type="VI" URL="../common/UVControl/UVControl_ConvertPaths.vi"/>
		</Item>
		<Item Name="UVSock" Type="Folder">
			<Item Name="Implementation" Type="Folder">
				<Item Name="DLL Interface" Type="Folder">
					<Item Name="UVSC_ReadAsyncQ.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_ReadAsyncQ.vi"/>
					<Item Name="UVSC_ReadBuildQ.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_ReadBuildQ.vi"/>
					<Item Name="UVSC_ReadPBarQ.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_ReadPBarQ.vi"/>
					<Item Name="UVSC_TxRxRaw.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_TxRxRaw.vi"/>
					<Item Name="UVSC_UnInit.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_UnInit.vi"/>
					<Item Name="UVSC_CloseConnection.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_CloseConnection.vi"/>
					<Item Name="UVSC_ConnHandleFromConnName.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_ConnHandleFromConnName.vi"/>
					<Item Name="UVSC_CreateMsg.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_CreateMsg.vi"/>
					<Item Name="UVSC_DBG_CALC_EXPRESSION.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_DBG_CALC_EXPRESSION.vi"/>
					<Item Name="UVSC_DBG_CHANGE_BP.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_DBG_CHANGE_BP.vi"/>
					<Item Name="UVSC_DBG_CREATE_BP.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_DBG_CREATE_BP.vi"/>
					<Item Name="UVSC_DBG_ENTER.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_DBG_ENTER.vi"/>
					<Item Name="UVSC_DBG_ENUMERATE_BP.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_DBG_ENUMERATE_BP.vi"/>
					<Item Name="UVSC_DBG_EXIT.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_DBG_EXIT.vi"/>
					<Item Name="UVSC_DBG_MEM_READ.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_DBG_MEM_READ.vi"/>
					<Item Name="UVSC_DBG_MEM_WRITE.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_DBG_MEM_WRITE.vi"/>
					<Item Name="UVSC_DBG_RESET.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_DBG_RESET.vi"/>
					<Item Name="UVSC_DBG_SERIAL_GET.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_DBG_SERIAL_GET.vi"/>
					<Item Name="UVSC_DBG_SERIAL_PUT.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_DBG_SERIAL_PUT.vi"/>
					<Item Name="UVSC_DBG_START_EXECUTION.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_DBG_START_EXECUTION.vi"/>
					<Item Name="UVSC_DBG_STATUS.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_DBG_STATUS.vi"/>
					<Item Name="UVSC_DBG_STEP_HLL.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_DBG_STEP_HLL.vi"/>
					<Item Name="UVSC_DBG_STEP_INSTRUCTION.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_DBG_STEP_INSTRUCTION.vi"/>
					<Item Name="UVSC_DBG_STEP_INTO.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_DBG_STEP_INTO.vi"/>
					<Item Name="UVSC_DBG_STEP_OUT.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_DBG_STEP_OUT.vi"/>
					<Item Name="UVSC_DBG_STOP_EXECUTION.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_DBG_STOP_EXECUTION.vi"/>
					<Item Name="UVSC_DBG_VTR_GET.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_DBG_VTR_GET.vi"/>
					<Item Name="UVSC_DBG_VTR_SET.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_DBG_VTR_SET.vi"/>
					<Item Name="UVSC_DLL_path.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_DLL_path.vi"/>
					<Item Name="UVSC_FlushAsyncQ.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_FlushAsyncQ.vi"/>
					<Item Name="UVSC_GEN_EXIT.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_GEN_EXIT.vi"/>
					<Item Name="UVSC_GEN_HIDE.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_GEN_HIDE.vi"/>
					<Item Name="UVSC_GEN_MAXIMIZE.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_GEN_MAXIMIZE.vi"/>
					<Item Name="UVSC_GEN_MINIMIZE.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_GEN_MINIMIZE.vi"/>
					<Item Name="UVSC_GEN_RESTORE.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_GEN_RESTORE.vi"/>
					<Item Name="UVSC_GEN_SHOW.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_GEN_SHOW.vi"/>
					<Item Name="UVSC_GEN_UVSOCK_VERSION.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_GEN_UVSOCK_VERSION.vi"/>
					<Item Name="UVSC_GEN_CHECK_LICENSE.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_GEN_CHECK_LICENSE.vi"/>
					<Item Name="UVSC_GetBuildOutput.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_GetBuildOutput.vi"/>
					<Item Name="UVSC_GetBuildOutputSize.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_GetBuildOutputSize.vi"/>
					<Item Name="UVSC_GetLastError.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_GetLastError.vi"/>
					<Item Name="UVSC_Init.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_Init.vi"/>
					<Item Name="UVSC_OpenConnection.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_OpenConnection.vi"/>
					<Item Name="UVSC_PRJ_ACTIVE_FILES.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_PRJ_ACTIVE_FILES.vi"/>
					<Item Name="UVSC_PRJ_ADD_FILE.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_PRJ_ADD_FILE.vi"/>
					<Item Name="UVSC_PRJ_ADD_GROUP.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_PRJ_ADD_GROUP.vi"/>
					<Item Name="UVSC_PRJ_BUILD.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_PRJ_BUILD.vi"/>
					<Item Name="UVSC_PRJ_BUILD_CANCEL.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_PRJ_BUILD_CANCEL.vi"/>
					<Item Name="UVSC_PRJ_CLEAN.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_PRJ_CLEAN.vi"/>
					<Item Name="UVSC_PRJ_CLOSE.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_PRJ_CLOSE.vi"/>
					<Item Name="UVSC_PRJ_DEL_FILE.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_PRJ_DEL_FILE.vi"/>
					<Item Name="UVSC_PRJ_DEL_GROUP.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_PRJ_DEL_GROUP.vi"/>
					<Item Name="UVSC_PRJ_ENUM_FILES.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_PRJ_ENUM_FILES.vi"/>
					<Item Name="UVSC_PRJ_ENUM_GROUPS.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_PRJ_ENUM_GROUPS.vi"/>
					<Item Name="UVSC_PRJ_FLASH_DOWNLOAD.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_PRJ_FLASH_DOWNLOAD.vi"/>
					<Item Name="UVSC_PRJ_GET_OPTITEM.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_PRJ_GET_OPTITEM.vi"/>
					<Item Name="UVSC_PRJ_LOAD.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_PRJ_LOAD.vi"/>
					<Item Name="UVSC_PRJ_SET_OPTITEM.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_PRJ_SET_OPTITEM.vi"/>
					<Item Name="UVSC_PRJ_SET_DEBUG_TARGET.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_PRJ_SET_DEBUG_TARGET.vi"/>
					<Item Name="UVSC_DBG_ADR_TOFILELINE.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_DBG_ADR_TOFILELINE.vi"/>
					<Item Name="UVSC_DBG_SET_CALLBACK.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_DBG_SET_CALLBACK.vi"/>
					<Item Name="UVSC_DBG_ENUM_VTR.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_DBG_ENUM_VTR.vi"/>
					<Item Name="UVSC_DBG_ENUM_STACK.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_DBG_ENUM_STACK.vi"/>
					<Item Name="UVSC_DBG_ADR_SHOWCODE.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_DBG_ADR_SHOWCODE.vi"/>
					<Item Name="UVSC_DBG_WAKE.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_DBG_WAKE.vi"/>
					<Item Name="UVSC_LogControl.vi" Type="VI" URL="../common/UVSock/implementation/DLL_Interface/UVSC_LogControl.vi"/>
				</Item>
				<Item Name="DataEncodeDecode" Type="Folder">
					<Item Name="UVSock_UI32Word2UI8Array.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_UI32Word2UI8Array.vi"/>
					<Item Name="UVSock_UI64Word2UI8Array.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_UI64Word2UI8Array.vi"/>
					<Item Name="UVSock_DecodeAsyncGeneric.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_DecodeAsyncGeneric.vi"/>
					<Item Name="UVSock_DecodeBPChange.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_DecodeBPChange.vi"/>
					<Item Name="UVSock_DecodeBPData.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_DecodeBPData.vi"/>
					<Item Name="UVSock_DecodeHeader.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_DecodeHeader.vi"/>
					<Item Name="UVSock_DecodeMemData.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_DecodeMemData.vi"/>
					<Item Name="UVSock_DecodeMemInfo.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_DecodeMemInfo.vi"/>
					<Item Name="UVSock_DecodePBar.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_DecodePBar.vi"/>
					<Item Name="UVSock_DecodePrjData.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_DecodePrjData.vi"/>
					<Item Name="UVSock_DecodeRspGeneric.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_DecodeRspGeneric.vi"/>
					<Item Name="UVSock_DecodeSerialData.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_DecodeSerialData.vi"/>
					<Item Name="UVSock_DecodeStatusData.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_DecodeStatusData.vi"/>
					<Item Name="UVSock_DecodeStrData.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_DecodeStrData.vi"/>
					<Item Name="UVSock_DecodeTrnOpt.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_DecodeTrnOpt.vi"/>
					<Item Name="UVSock_DecodeTvalData.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_DecodeTvalData.vi"/>
					<Item Name="UVSock_EncodeBPChange.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_EncodeBPChange.vi"/>
					<Item Name="UVSock_EncodeBPData.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_EncodeBPData.vi"/>
					<Item Name="UVSock_EncodeMemData.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_EncodeMemData.vi"/>
					<Item Name="UVSock_EncodePrjData.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_EncodePrjData.vi"/>
					<Item Name="UVSock_EncodeSerialData.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_EncodeSerialData.vi"/>
					<Item Name="UVSock_EncodeStrData.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_EncodeStrData.vi"/>
					<Item Name="UVSock_EncodeTrnOpt.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_EncodeTrnOpt.vi"/>
					<Item Name="UVSock_EncodeTvalData.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_EncodeTvalData.vi"/>
					<Item Name="UVSock_UI8Array2CmdType.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_UI8Array2CmdType.vi"/>
					<Item Name="UVSock_UI8Array2UI16Word.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_UI8Array2UI16Word.vi"/>
					<Item Name="UVSock_UI8Array2UI32Word.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_UI8Array2UI32Word.vi"/>
					<Item Name="UVSock_UI8Array2UI64Word.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_UI8Array2UI64Word.vi"/>
					<Item Name="UVSock_UI16Word2UI8Array.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_UI16Word2UI8Array.vi"/>
					<Item Name="UVSock_EncodeAdrMtflData.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_EncodeAdrMtflData.vi"/>
					<Item Name="UVSock_DecodeAflMapData.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_DecodeAflMapData.vi"/>
					<Item Name="UVSock_EncodeTmrCb.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_EncodeTmrCb.vi"/>
					<Item Name="UVSock_EncodeiVtrEnum.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_EncodeiVtrEnum.vi"/>
					<Item Name="UVSock_DecodeStackEnum.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_DecodeStackEnum.vi"/>
					<Item Name="UVSock_DecodeAVtr.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_DecodeAVtr.vi"/>
					<Item Name="UVSock_EncodeiStkEnum.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_EncodeiStkEnum.vi"/>
					<Item Name="UVSock_DecodeiShowSync.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_DecodeiShowSync.vi"/>
					<Item Name="UVSock_EncodeiShowSync.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_EncodeiShowSync.vi"/>
				</Item>
				<Item Name="Legacy" Type="Folder">
					<Item Name="UVSock_TxRx.vi" Type="VI" URL="../common/UVSock/implementation/Legacy/UVSock_TxRx.vi"/>
					<Item Name="UVSock_MsgLog.vi" Type="VI" URL="../common/UVSock/implementation/Legacy/UVSock_MsgLog.vi"/>
					<Item Name="MsgLogFileRef.vi" Type="VI" URL="../common/UVSock/implementation/Legacy/MsgLogFileRef.vi"/>
					<Item Name="UVSock_MsgLog_Open.vi" Type="VI" URL="../common/UVSock/implementation/Legacy/UVSock_MsgLog_Open.vi"/>
				</Item>
				<Item Name="UVSock_PRJ_BUILD_StartTask.vi" Type="VI" URL="../common/UVSock/implementation/UVSock_PRJ_BUILD_StartTask.vi"/>
				<Item Name="UVSock_PRJ_BUILD_StopTask.vi" Type="VI" URL="../common/UVSock/implementation/UVSock_PRJ_BUILD_StopTask.vi"/>
				<Item Name="UVSock_PRJ_BUILD_Task.vi" Type="VI" URL="../common/UVSock/implementation/UVSock_PRJ_BUILD_Task.vi"/>
				<Item Name="UVSock_PRJ_FLASH_DOWNLOAD_StartTask.vi" Type="VI" URL="../common/UVSock/implementation/UVSock_PRJ_FLASH_DOWNLOAD_StartTask.vi"/>
				<Item Name="UVSock_PRJ_FLASH_DOWNLOAD_StopTask.vi" Type="VI" URL="../common/UVSock/implementation/UVSock_PRJ_FLASH_DOWNLOAD_StopTask.vi"/>
				<Item Name="UVSock_PRJ_FLASH_DOWNLOAD_Task.vi" Type="VI" URL="../common/UVSock/implementation/UVSock_PRJ_FLASH_DOWNLOAD_Task.vi"/>
				<Item Name="UVSock_RxAsync.vi" Type="VI" URL="../common/UVSock/implementation/UVSock_RxAsync.vi"/>
				<Item Name="UVSock_Start_Daemon.vi" Type="VI" URL="../common/UVSock/implementation/UVSock_Start_Daemon.vi"/>
				<Item Name="UVSock_Stop_Daemon.vi" Type="VI" URL="../common/UVSock/implementation/UVSock_Stop_Daemon.vi"/>
				<Item Name="UVSock_Daemon.vi" Type="VI" URL="../common/UVSock/implementation/UVSock_Daemon.vi"/>
				<Item Name="UVSock_GetLastError.vi" Type="VI" URL="../common/UVSock/implementation/UVSock_GetLastError.vi"/>
			</Item>
			<Item Name="UVSock_Start.vi" Type="VI" URL="../common/UVSock/UVSock_Start.vi"/>
			<Item Name="UVSock_Stop.vi" Type="VI" URL="../common/UVSock/UVSock_Stop.vi"/>
			<Item Name="UVSock_TEST_1.vi" Type="VI" URL="../common/UVSock/UVSock_TEST_1.vi"/>
			<Item Name="UVSock_TEST_2.vi" Type="VI" URL="../common/UVSock/UVSock_TEST_2.vi"/>
			<Item Name="UVSock_TESTER.vi" Type="VI" URL="../common/UVSock/UVSock_TESTER.vi"/>
			<Item Name="UVSock_Close.vi" Type="VI" URL="../common/UVSock/UVSock_Close.vi"/>
			<Item Name="UVSock_DBG_CALC_EXPRESSION.vi" Type="VI" URL="../common/UVSock/UVSock_DBG_CALC_EXPRESSION.vi"/>
			<Item Name="UVSock_DBG_CHANGE_BP.vi" Type="VI" URL="../common/UVSock/UVSock_DBG_CHANGE_BP.vi"/>
			<Item Name="UVSock_DBG_CLEAR_ALL_BP.vi" Type="VI" URL="../common/UVSock/UVSock_DBG_CLEAR_ALL_BP.vi"/>
			<Item Name="UVSock_DBG_CREATE_BP.vi" Type="VI" URL="../common/UVSock/UVSock_DBG_CREATE_BP.vi"/>
			<Item Name="UVSock_DBG_ENTER.vi" Type="VI" URL="../common/UVSock/UVSock_DBG_ENTER.vi"/>
			<Item Name="UVSock_DBG_ENUMERATE_BP.vi" Type="VI" URL="../common/UVSock/UVSock_DBG_ENUMERATE_BP.vi"/>
			<Item Name="UVSock_DBG_EXIT.vi" Type="VI" URL="../common/UVSock/UVSock_DBG_EXIT.vi"/>
			<Item Name="UVSock_DBG_MEM_READ.vi" Type="VI" URL="../common/UVSock/UVSock_DBG_MEM_READ.vi"/>
			<Item Name="UVSock_DBG_MEM_WRITE.vi" Type="VI" URL="../common/UVSock/UVSock_DBG_MEM_WRITE.vi"/>
			<Item Name="UVSock_DBG_RESET.vi" Type="VI" URL="../common/UVSock/UVSock_DBG_RESET.vi"/>
			<Item Name="UVSock_DBG_SERIAL_GET.vi" Type="VI" URL="../common/UVSock/UVSock_DBG_SERIAL_GET.vi"/>
			<Item Name="UVSock_DBG_SERIAL_PUT.vi" Type="VI" URL="../common/UVSock/UVSock_DBG_SERIAL_PUT.vi"/>
			<Item Name="UVSock_DBG_START_EXECUTION.vi" Type="VI" URL="../common/UVSock/UVSock_DBG_START_EXECUTION.vi"/>
			<Item Name="UVSock_DBG_STATUS.vi" Type="VI" URL="../common/UVSock/UVSock_DBG_STATUS.vi"/>
			<Item Name="UVSock_DBG_STEP_HLL.vi" Type="VI" URL="../common/UVSock/UVSock_DBG_STEP_HLL.vi"/>
			<Item Name="UVSock_DBG_STEP_INSTRUCTION.vi" Type="VI" URL="../common/UVSock/UVSock_DBG_STEP_INSTRUCTION.vi"/>
			<Item Name="UVSock_DBG_STEP_INTO.vi" Type="VI" URL="../common/UVSock/UVSock_DBG_STEP_INTO.vi"/>
			<Item Name="UVSock_DBG_STEP_OUT.vi" Type="VI" URL="../common/UVSock/UVSock_DBG_STEP_OUT.vi"/>
			<Item Name="UVSock_DBG_STOP_EXECUTION.vi" Type="VI" URL="../common/UVSock/UVSock_DBG_STOP_EXECUTION.vi"/>
			<Item Name="UVSock_DBG_VTR_GET.vi" Type="VI" URL="../common/UVSock/UVSock_DBG_VTR_GET.vi"/>
			<Item Name="UVSock_DBG_VTR_SET.vi" Type="VI" URL="../common/UVSock/UVSock_DBG_VTR_SET.vi"/>
			<Item Name="UVSock_DBG_ADR_TOFILELINE.vi" Type="VI" URL="../common/UVSock/UVSock_DBG_ADR_TOFILELINE.vi"/>
			<Item Name="UVSock_GEN_EXIT.vi" Type="VI" URL="../common/UVSock/UVSock_GEN_EXIT.vi"/>
			<Item Name="UVSock_GEN_HIDE.vi" Type="VI" URL="../common/UVSock/UVSock_GEN_HIDE.vi"/>
			<Item Name="UVSock_GEN_MAXIMIZE.vi" Type="VI" URL="../common/UVSock/UVSock_GEN_MAXIMIZE.vi"/>
			<Item Name="UVSock_GEN_MINIMIZE.vi" Type="VI" URL="../common/UVSock/UVSock_GEN_MINIMIZE.vi"/>
			<Item Name="UVSock_GEN_RESTORE.vi" Type="VI" URL="../common/UVSock/UVSock_GEN_RESTORE.vi"/>
			<Item Name="UVSock_GEN_SHOW.vi" Type="VI" URL="../common/UVSock/UVSock_GEN_SHOW.vi"/>
			<Item Name="UVSock_GET_VERSION.vi" Type="VI" URL="../common/UVSock/UVSock_GET_VERSION.vi"/>
			<Item Name="UVSock_GEN_CHECK_LICENSE.vi" Type="VI" URL="../common/UVSock/UVSock_GEN_CHECK_LICENSE.vi"/>
			<Item Name="UVSock_GetBuildOutput.vi" Type="VI" URL="../common/UVSock/UVSock_GetBuildOutput.vi"/>
			<Item Name="UVSock_PATH.vi" Type="VI" URL="../common/UVSock/UVSock_PATH.vi"/>
			<Item Name="UVSock_PRJ_ACTIVE_FILES.vi" Type="VI" URL="../common/UVSock/UVSock_PRJ_ACTIVE_FILES.vi"/>
			<Item Name="UVSock_PRJ_ADD_FILE.vi" Type="VI" URL="../common/UVSock/UVSock_PRJ_ADD_FILE.vi"/>
			<Item Name="UVSock_PRJ_ADD_GROUP.vi" Type="VI" URL="../common/UVSock/UVSock_PRJ_ADD_GROUP.vi"/>
			<Item Name="UVSock_PRJ_BUILD.vi" Type="VI" URL="../common/UVSock/UVSock_PRJ_BUILD.vi"/>
			<Item Name="UVSock_PRJ_BUILD_CANCEL.vi" Type="VI" URL="../common/UVSock/UVSock_PRJ_BUILD_CANCEL.vi"/>
			<Item Name="UVSock_PRJ_CLEAN.vi" Type="VI" URL="../common/UVSock/UVSock_PRJ_CLEAN.vi"/>
			<Item Name="UVSock_PRJ_CLOSE.vi" Type="VI" URL="../common/UVSock/UVSock_PRJ_CLOSE.vi"/>
			<Item Name="UVSock_PRJ_DEL_FILE.vi" Type="VI" URL="../common/UVSock/UVSock_PRJ_DEL_FILE.vi"/>
			<Item Name="UVSock_PRJ_DEL_GROUP.vi" Type="VI" URL="../common/UVSock/UVSock_PRJ_DEL_GROUP.vi"/>
			<Item Name="UVSock_PRJ_ENUM_FILES.vi" Type="VI" URL="../common/UVSock/UVSock_PRJ_ENUM_FILES.vi"/>
			<Item Name="UVSock_PRJ_ENUM_GROUPS.vi" Type="VI" URL="../common/UVSock/UVSock_PRJ_ENUM_GROUPS.vi"/>
			<Item Name="UVSock_PRJ_FLASH_DOWNLOAD.vi" Type="VI" URL="../common/UVSock/UVSock_PRJ_FLASH_DOWNLOAD.vi"/>
			<Item Name="UVSock_PRJ_GET_CODEGEN.vi" Type="VI" URL="../common/UVSock/UVSock_PRJ_GET_CODEGEN.vi"/>
			<Item Name="UVSock_PRJ_GET_MEMINFO.vi" Type="VI" URL="../common/UVSock/UVSock_PRJ_GET_MEMINFO.vi"/>
			<Item Name="UVSock_PRJ_GET_OPTIMIZATION.vi" Type="VI" URL="../common/UVSock/UVSock_PRJ_GET_OPTIMIZATION.vi"/>
			<Item Name="UVSock_PRJ_GET_OPTITEM.vi" Type="VI" URL="../common/UVSock/UVSock_PRJ_GET_OPTITEM.vi"/>
			<Item Name="UVSock_PRJ_LOAD.vi" Type="VI" URL="../common/UVSock/UVSock_PRJ_LOAD.vi"/>
			<Item Name="UVSock_PRJ_SET_CODEGEN.vi" Type="VI" URL="../common/UVSock/UVSock_PRJ_SET_CODEGEN.vi"/>
			<Item Name="UVSock_PRJ_SET_OPTIMIZATION.vi" Type="VI" URL="../common/UVSock/UVSock_PRJ_SET_OPTIMIZATION.vi"/>
			<Item Name="UVSock_PRJ_SET_OPTITEM.vi" Type="VI" URL="../common/UVSock/UVSock_PRJ_SET_OPTITEM.vi"/>
			<Item Name="UVSock_PRJ_SET_DEBUG_TARGET.vi" Type="VI" URL="../common/UVSock/UVSock_PRJ_SET_DEBUG_TARGET.vi"/>
			<Item Name="UVSock_DBG_SET_CALLBACK.vi" Type="VI" URL="../common/UVSock/UVSock_DBG_SET_CALLBACK.vi"/>
			<Item Name="UVSock_DBG_ENUM_VTR.vi" Type="VI" URL="../common/UVSock/UVSock_DBG_ENUM_VTR.vi"/>
			<Item Name="UVSock_DBG_ENUM_STACK.vi" Type="VI" URL="../common/UVSock/UVSock_DBG_ENUM_STACK.vi"/>
			<Item Name="UVSock_DBG_ENUM_STACK_EX.vi" Type="VI" URL="../common/UVSock/UVSock_DBG_ENUM_STACK_EX.vi"/>
			<Item Name="UVSock_DBG_ADR_SHOWCODE.vi" Type="VI" URL="../common/UVSock/UVSock_DBG_ADR_SHOWCODE.vi"/>
			<Item Name="UVSock_DBG_WAKE.vi" Type="VI" URL="../common/UVSock/UVSock_DBG_WAKE.vi"/>
		</Item>
		<Item Name="OCDI" Type="Folder">
			<Item Name="util" Type="Folder">
				<Item Name="EMB_Debug_OCDI_RV_Indication.ctl" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/util/EMB_Debug_OCDI_RV_Indication.ctl"/>
				<Item Name="EMB_Debug_OCDI_RV_Indication.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/util/EMB_Debug_OCDI_RV_Indication.vi"/>
				<Item Name="EMB_Debug_OCDI_RV_Indication_Global.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/util/EMB_Debug_OCDI_RV_Indication_Global.vi"/>
				<Item Name="EMB_Debug_OCDI_RV_LastBPAddess_Global.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/util/EMB_Debug_OCDI_RV_LastBPAddess_Global.vi"/>
				<Item Name="EMB_Debug_OCDI_RV_MakeError.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/util/EMB_Debug_OCDI_RV_MakeError.vi"/>
				<Item Name="EMB_Debug_OCDI_RV_MemOps.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/util/EMB_Debug_OCDI_RV_MemOps.vi"/>
				<Item Name="EMB_Debug_OCDI_RV_OpenConnection.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/util/EMB_Debug_OCDI_RV_OpenConnection.vi"/>
				<Item Name="EMB_Debug_OCDI_RV_Poll.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/util/EMB_Debug_OCDI_RV_Poll.vi"/>
				<Item Name="EMB_Debug_OCDI_RV_Prepare.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/util/EMB_Debug_OCDI_RV_Prepare.vi"/>
				<Item Name="EMB_Debug_OCDI_RV_Reply.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/util/EMB_Debug_OCDI_RV_Reply.vi"/>
				<Item Name="EMB_Debug_OCDI_RV_Reply_Codes.ctl" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/util/EMB_Debug_OCDI_RV_Reply_Codes.ctl"/>
				<Item Name="EMB_Debug_OCDI_RV_Request.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/util/EMB_Debug_OCDI_RV_Request.vi"/>
				<Item Name="EMB_Debug_OCDI_RV_Request_Codes.ctl" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/util/EMB_Debug_OCDI_RV_Request_Codes.ctl"/>
				<Item Name="EMB_Debug_OCDI_RV_Session.ctl" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/util/EMB_Debug_OCDI_RV_Session.ctl"/>
				<Item Name="EMB_Debug_OCDI_RV_Session.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/util/EMB_Debug_OCDI_RV_Session.vi"/>
				<Item Name="EMB_Debug_OCDI_RV_StartEclipse.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/util/EMB_Debug_OCDI_RV_StartEclipse.vi"/>
				<Item Name="EMB_Debug_OCDI_RV_Thread.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/util/EMB_Debug_OCDI_RV_Thread.vi"/>
				<Item Name="EMB_Debug_OCDI_RV_Wait_Indication.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/util/EMB_Debug_OCDI_RV_Wait_Indication.vi"/>
				<Item Name="EMB_Debug_OCDI_RV_Crash_Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/util/EMB_Debug_OCDI_RV_Crash_Dialog.vi"/>
				<Item Name="EMB_Debug_OCDI_RV_Errors.ctl" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/util/EMB_Debug_OCDI_RV_Errors.ctl"/>
				<Item Name="EMB_Debug_OCDI_RV_GetConfig.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/util/EMB_Debug_OCDI_RV_GetConfig.vi"/>
				<Item Name="EMB_Debug_OCDI_RV_RunTarget.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/util/EMB_Debug_OCDI_RV_RunTarget.vi"/>
				<Item Name="EMB_Debug_OCDI_RV_BPStore.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/util/EMB_Debug_OCDI_RV_BPStore.vi"/>
			</Item>
			<Item Name="EMB_Debug_OCDI_RV_ClearBreakpoints.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/EMB_Debug_OCDI_RV_ClearBreakpoints.vi"/>
			<Item Name="EMB_Debug_OCDI_RV_Config.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/EMB_Debug_OCDI_RV_Config.vi"/>
			<Item Name="EMB_Debug_OCDI_RV_Continue.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/EMB_Debug_OCDI_RV_Continue.vi"/>
			<Item Name="EMB_Debug_OCDI_RV_Download.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/EMB_Debug_OCDI_RV_Download.vi"/>
			<Item Name="EMB_Debug_OCDI_RV_Exit.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/EMB_Debug_OCDI_RV_Exit.vi"/>
			<Item Name="EMB_Debug_OCDI_RV_GetBreakpoint.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/EMB_Debug_OCDI_RV_GetBreakpoint.vi"/>
			<Item Name="EMB_Debug_OCDI_RV_GetIndication.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/EMB_Debug_OCDI_RV_GetIndication.vi"/>
			<Item Name="EMB_Debug_OCDI_RV_Go.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/EMB_Debug_OCDI_RV_Go.vi"/>
			<Item Name="EMB_Debug_OCDI_RV_Init.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/EMB_Debug_OCDI_RV_Init.vi"/>
			<Item Name="EMB_Debug_OCDI_RV_InitIndication.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/EMB_Debug_OCDI_RV_InitIndication.vi"/>
			<Item Name="EMB_Debug_OCDI_RV_MemRead.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/EMB_Debug_OCDI_RV_MemRead.vi"/>
			<Item Name="EMB_Debug_OCDI_RV_MemWrite.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/EMB_Debug_OCDI_RV_MemWrite.vi"/>
			<Item Name="EMB_Debug_OCDI_RV_Pause.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/EMB_Debug_OCDI_RV_Pause.vi"/>
			<Item Name="EMB_Debug_OCDI_RV_Release.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/EMB_Debug_OCDI_RV_Release.vi"/>
			<Item Name="EMB_Debug_OCDI_RV_ResolveSymbols.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/EMB_Debug_OCDI_RV_ResolveSymbols.vi"/>
			<Item Name="EMB_Debug_OCDI_RV_SetBreakpoints.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/EMB_Debug_OCDI_RV_SetBreakpoints.vi"/>
			<Item Name="EMB_Debug_OCDI_RV_ShowCrash.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/EMB_Debug_OCDI_RV_ShowCrash.vi"/>
			<Item Name="EMB_Debug_OCDI_RV_ShowSource.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/EMB_Debug_OCDI_RV_ShowSource.vi"/>
			<Item Name="EMB_Debug_OCDI_RV_Stop.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/RealView/EMB_Debug_OCDI_RV_Stop.vi"/>
		</Item>
		<Item Name="Project" Type="Folder"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="Trim Whitespace.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Trim Whitespace.vi"/>
				<Item Name="whitespace.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/whitespace.ctl"/>
				<Item Name="Clear Errors.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Clear Errors.vi"/>
				<Item Name="LEP_Utilities_GetProjectInfo.vi" Type="VI" URL="/&lt;vilib&gt;/Platform/EmbProject/Plugins/utilities/LEP_Utilities_GetProjectInfo.vi"/>
				<Item Name="NI_FileType.lvlib" Type="Library" URL="/&lt;vilib&gt;/Utility/lvfile.llb/NI_FileType.lvlib"/>
				<Item Name="Error Cluster From Error Code.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Error Cluster From Error Code.vi"/>
				<Item Name="LEP_Utilities_GetTargetInfo.vi" Type="VI" URL="/&lt;vilib&gt;/Platform/EmbProject/Plugins/utilities/LEP_Utilities_GetTargetInfo.vi"/>
				<Item Name="LEP_Utilities_GetBuildSpecInfo.vi" Type="VI" URL="/&lt;vilib&gt;/Platform/EmbProject/Plugins/utilities/LEP_Utilities_GetBuildSpecInfo.vi"/>
				<Item Name="LEP_Utilities_TempData.vi" Type="VI" URL="/&lt;vilib&gt;/Platform/EmbProject/Plugins/utilities/LEP_Utilities_TempData.vi"/>
				<Item Name="LEP_Utilities_GetSetProjectData.vi" Type="VI" URL="/&lt;vilib&gt;/Platform/EmbProject/Plugins/utilities/LEP_Utilities_GetSetProjectData.vi"/>
				<Item Name="LEP_Utilities_GetSetProjectData_Variant.vi" Type="VI" URL="/&lt;vilib&gt;/Platform/EmbProject/Plugins/utilities/LEP_Utilities_GetSetProjectData_Variant.vi"/>
				<Item Name="LEP_Utilities_GetSetProjectData_U32.vi" Type="VI" URL="/&lt;vilib&gt;/Platform/EmbProject/Plugins/utilities/LEP_Utilities_GetSetProjectData_U32.vi"/>
				<Item Name="LEP_Utilities_GetSetProjectData_I32.vi" Type="VI" URL="/&lt;vilib&gt;/Platform/EmbProject/Plugins/utilities/LEP_Utilities_GetSetProjectData_I32.vi"/>
				<Item Name="LEP_Utilities_GetSetProjectData_String.vi" Type="VI" URL="/&lt;vilib&gt;/Platform/EmbProject/Plugins/utilities/LEP_Utilities_GetSetProjectData_String.vi"/>
				<Item Name="LEP_Utilities_GetSetProjectData_Path.vi" Type="VI" URL="/&lt;vilib&gt;/Platform/EmbProject/Plugins/utilities/LEP_Utilities_GetSetProjectData_Path.vi"/>
				<Item Name="EMB_Utility_AppendFileList.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Utilities/FileList/EMB_Utility_AppendFileList.vi"/>
				<Item Name="LVDData.dll" Type="Document" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/utils/LVDData.dll"/>
				<Item Name="EMB_Debug_DD_MakeError.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/utils/EMB_Debug_DD_MakeError.vi"/>
				<Item Name="EMB_Debug_DD_Error.ctl" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/utils/EMB_Debug_DD_Error.ctl"/>
				<Item Name="LEP_Utilities_GetContext.vi" Type="VI" URL="/&lt;vilib&gt;/Platform/EmbProject/Plugins/utilities/LEP_Utilities_GetContext.vi"/>
				<Item Name="LEP_Utilities_GetProjectISRVIs.vi" Type="VI" URL="/&lt;vilib&gt;/Platform/EmbProject/utilities/LEP_Utilities_GetProjectISRVIs.vi"/>
				<Item Name="LEP_Utilities_CheckISRConnectorPane.vi" Type="VI" URL="/&lt;vilib&gt;/Platform/EmbProject/utilities/LEP_Utilities_CheckISRConnectorPane.vi"/>
				<Item Name="EMB_CGen_SetParams.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Utilities/CGen/EMB_CGen_SetParams.vi"/>
				<Item Name="EMB_Utility_AppendVIList.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Utilities/FileList/EMB_Utility_AppendVIList.vi"/>
				<Item Name="EMB_Debug_DD_GetSetAttrib.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/EMB_Debug_DD_GetSetAttrib.vi"/>
				<Item Name="EMB_Debug_DD_GetSetAttrib_Numeric.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/utils/EMB_Debug_DD_GetSetAttrib_Numeric.vi"/>
				<Item Name="EMB_Debug_DD_GetSetAttrib_String.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/utils/EMB_Debug_DD_GetSetAttrib_String.vi"/>
				<Item Name="EMB_Debug_DD_GetSetAttrib_Numeric_Pair.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/utils/EMB_Debug_DD_GetSetAttrib_Numeric_Pair.vi"/>
				<Item Name="Check if File or Folder Exists.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/libraryn.llb/Check if File or Folder Exists.vi"/>
				<Item Name="Error to Warning.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Error to Warning.vi"/>
				<Item Name="System Exec.vi" Type="VI" URL="/&lt;vilib&gt;/Platform/system.llb/System Exec.vi"/>
				<Item Name="EMB_Utility_GetFileList.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Utilities/FileList/EMB_Utility_GetFileList.vi"/>
				<Item Name="EMB_Debug_DD_ParseCFile.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/EMB_Debug_DD_ParseCFile.vi"/>
				<Item Name="EMB_Debug_DD_VIMap.ctl" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/EMB_Debug_DD_VIMap.ctl"/>
				<Item Name="EMB_Debug_DD_VIMap.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/EMB_Debug_DD_VIMap.vi"/>
				<Item Name="EMB_Debug_DD_ParseBPString.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/utils/EMB_Debug_DD_ParseBPString.vi"/>
				<Item Name="EMB_Debug_DD_BPType.ctl" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/utils/EMB_Debug_DD_BPType.ctl"/>
				<Item Name="EMB_Debug_DD_InsertBP.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/EMB_Debug_DD_InsertBP.vi"/>
				<Item Name="EMB_Debug_DD_ParseHeapString.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/utils/EMB_Debug_DD_ParseHeapString.vi"/>
				<Item Name="EMB_Debug_DD_ParseSignalString.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/utils/EMB_Debug_DD_ParseSignalString.vi"/>
				<Item Name="EMB_Debug_DD_InsertSignal.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/EMB_Debug_DD_InsertSignal.vi"/>
				<Item Name="EMB_Debug_DD_ParseControlDataString.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/utils/EMB_Debug_DD_ParseControlDataString.vi"/>
				<Item Name="EMB_Debug_DD_ParseControlIDString.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/utils/EMB_Debug_DD_ParseControlIDString.vi"/>
				<Item Name="EMB_Debug_DD_InsertControl.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/EMB_Debug_DD_InsertControl.vi"/>
				<Item Name="EMB_Debug_DD_ParseGlobaControlData.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/utils/EMB_Debug_DD_ParseGlobaControlData.vi"/>
				<Item Name="EMB_Debug_DD_ParseNodeMarkerString.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/utils/EMB_Debug_DD_ParseNodeMarkerString.vi"/>
				<Item Name="EMB_Debug_DD_InsertEndPoint.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/EMB_Debug_DD_InsertEndPoint.vi"/>
				<Item Name="EMB_Debug_DD_InsertBeginPoint.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/EMB_Debug_DD_InsertBeginPoint.vi"/>
				<Item Name="EMB_Debug_DD_RebuildIndexTables.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/utils/EMB_Debug_DD_RebuildIndexTables.vi"/>
				<Item Name="EMB_Debug_DD_GetMemIDList.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/utils/EMB_Debug_DD_GetMemIDList.vi"/>
				<Item Name="EMB_Debug_DD_GetMem.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/EMB_Debug_DD_GetMem.vi"/>
				<Item Name="EMB_Debug_DD_MemType.ctl" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/utils/EMB_Debug_DD_MemType.ctl"/>
				<Item Name="EMB_Debug_DD_SetAbsoluteMemAddress.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/EMB_Debug_DD_SetAbsoluteMemAddress.vi"/>
				<Item Name="EMB_Debug_DD_SetRelativeMemAddress.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/EMB_Debug_DD_SetRelativeMemAddress.vi"/>
				<Item Name="EMB_Debug_DD_GetBPIDList.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/utils/EMB_Debug_DD_GetBPIDList.vi"/>
				<Item Name="EMB_Debug_DD_GetBP.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/EMB_Debug_DD_GetBP.vi"/>
				<Item Name="EMB_Debug_DD_SetAbsoluteBPAddress.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/EMB_Debug_DD_SetAbsoluteBPAddress.vi"/>
				<Item Name="EMB_Debug_DD_SetRelativeBPAddress.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/EMB_Debug_DD_SetRelativeBPAddress.vi"/>
				<Item Name="EMB_Utility_ReplaceBadChars.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Utilities/EMB_Utility_ReplaceBadChars.vi"/>
				<Item Name="EMB_Debug_DD_GetVIByIndex.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/utils/EMB_Debug_DD_GetVIByIndex.vi"/>
				<Item Name="Wait at Rendezvous.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/rendezvs.llb/Wait at Rendezvous.vi"/>
				<Item Name="Three Button Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Three Button Dialog.vi"/>
				<Item Name="Three Button Dialog CORE.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Three Button Dialog CORE.vi"/>
				<Item Name="Longest Line Length in Pixels.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Longest Line Length in Pixels.vi"/>
				<Item Name="Convert property node font to graphics font.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Convert property node font to graphics font.vi"/>
				<Item Name="Get Text Rect.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Get Text Rect.vi"/>
				<Item Name="nitargetDataType.ctl" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/TargetShared/nitargetDataType.ctl"/>
				<Item Name="EMB_Utility_MergePath.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Utilities/EMB_Utility_MergePath.vi"/>
				<Item Name="nitargetReadData.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/TargetShared/nitargetReadData.vi"/>
				<Item Name="EMB_Debug_OCDI_Indication_Codes.ctl" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/Interface/EMB_Debug_OCDI_Indication_Codes.ctl"/>
				<Item Name="EMB_Debug_OCDI_Eclipse_Request_Codes.ctl" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/Eclipse/util/EMB_Debug_OCDI_Eclipse_Request_Codes.ctl"/>
				<Item Name="Process Session Exec.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Utilities/ProcessSession/ProcessSession.llb/Process Session Exec.vi"/>
				<Item Name="Open Process Session.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Utilities/ProcessSession/ProcessSession.llb/Open Process Session.vi"/>
				<Item Name="ProcessSession.dll" Type="Document" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Utilities/ProcessSession/ProcessSession.dll"/>
				<Item Name="Process Session Status.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Utilities/ProcessSession/ProcessSession.llb/Process Session Status.vi"/>
				<Item Name="Kill Process Session .vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Utilities/ProcessSession/ProcessSession.llb/Kill Process Session .vi"/>
				<Item Name="Process Session Exit Code.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Utilities/ProcessSession/ProcessSession.llb/Process Session Exit Code.vi"/>
				<Item Name="Close Process Session.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Utilities/ProcessSession/ProcessSession.llb/Close Process Session.vi"/>
				<Item Name="EMB_Debug_OCDI_Eclipse_Request.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/Eclipse/util/EMB_Debug_OCDI_Eclipse_Request.vi"/>
				<Item Name="EMB_Debug_OCDI_Eclipse_Session.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/Eclipse/util/EMB_Debug_OCDI_Eclipse_Session.vi"/>
				<Item Name="EMB_Debug_DD_GetDDBreakpointAddress.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/EMB_Debug_DD_GetDDBreakpointAddress.vi"/>
				<Item Name="EMB_Debug_DD_BPAddressToID.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/EMB_Debug_DD_BPAddressToID.vi"/>
				<Item Name="EMB_Debug_DD_GetVIByCFileName.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/utils/EMB_Debug_DD_GetVIByCFileName.vi"/>
				<Item Name="EMB_Debug_DD_GetNodeByCLine.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/EMB_Debug_DD_GetNodeByCLine.vi"/>
				<Item Name="EMB_Debug_DD_BPIndexToID.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/EMB_Debug_DD_BPIndexToID.vi"/>
				<Item Name="EMB_Debug_DD_RebuildAddressTable.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/utils/EMB_Debug_DD_RebuildAddressTable.vi"/>
				<Item Name="EMB_Debug_DD_IDToBPAddress.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/EMB_Debug_DD_IDToBPAddress.vi"/>
				<Item Name="EMB_Debug_DD_GetFirstValidNodeFromCStack.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/EMB_Debug_DD_GetFirstValidNodeFromCStack.vi"/>
				<Item Name="nitargetSetUpdatePeriod.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/TargetShared/nitargetSetUpdatePeriod.vi"/>
				<Item Name="Rendezvous RefNum" Type="VI" URL="/&lt;vilib&gt;/Utility/rendezvs.llb/Rendezvous RefNum"/>
				<Item Name="EMB_Debug_Daemon_Config.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/Daemon/EMB_Debug_Daemon_Config.vi"/>
				<Item Name="EMB_Debug_Daemon_Errors.ctl" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/Daemon/utils/EMB_Debug_Daemon_Errors.ctl"/>
				<Item Name="EMB_Debug_OCDI_IConfig.ctl" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/Interface/EMB_Debug_OCDI_IConfig.ctl"/>
				<Item Name="EMB_Debug_Daemon_GetOCDIPath.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/Daemon/utils/EMB_Debug_Daemon_GetOCDIPath.vi"/>
				<Item Name="EMB_Debug_Daemon_Error.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/Daemon/utils/EMB_Debug_Daemon_Error.vi"/>
				<Item Name="EMB_Debug_OCDI_Eclipse_Reply_Codes.ctl" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/Eclipse/util/EMB_Debug_OCDI_Eclipse_Reply_Codes.ctl"/>
				<Item Name="EMB_Debug_OCDI_Eclipse_Reply.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/Eclipse/util/EMB_Debug_OCDI_Eclipse_Reply.vi"/>
				<Item Name="EMB_Debug_OCDI_Session.ctl" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/OCDI/Interface/EMB_Debug_OCDI_Session.ctl"/>
				<Item Name="EMB_Utility_MemMap_VI_Compare.ctl" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Utilities/MemMap/EMB_Utility_MemMap_VI_Compare.ctl"/>
				<Item Name="EMB_Utility_MemMap_VI_Default.ctl" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Utilities/MemMap/EMB_Utility_MemMap_VI_Default.ctl"/>
				<Item Name="EMB_Utility_MemMap_VI_Config.ctl" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Utilities/MemMap/EMB_Utility_MemMap_VI_Config.ctl"/>
				<Item Name="EMB_Utility_MemMap_VI_Type.ctl" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Utilities/MemMap/EMB_Utility_MemMap_VI_Type.ctl"/>
				<Item Name="EMB_Utility_MemMap_Init.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Utilities/MemMap/EMB_Utility_MemMap_Init.vi"/>
				<Item Name="EMB_Utility_MemMap.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Utilities/MemMap/EMB_Utility_MemMap.vi"/>
				<Item Name="EMB_Utility_MemMap_Edit.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Utilities/MemMap/EMB_Utility_MemMap_Edit.vi"/>
				<Item Name="EMB_Debug_DD_GetNodeLineRageFromInfo.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/Debug/DD/EMB_Debug_DD_GetNodeLineRageFromInfo.vi"/>
				<Item Name="Generate Temporary File Path.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/libraryn.llb/Generate Temporary File Path.vi"/>
				<Item Name="Simple Error Handler.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Simple Error Handler.vi"/>
				<Item Name="DialogType.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/DialogType.ctl"/>
				<Item Name="General Error Handler.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/General Error Handler.vi"/>
				<Item Name="DialogTypeEnum.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/DialogTypeEnum.ctl"/>
				<Item Name="General Error Handler CORE.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/General Error Handler CORE.vi"/>
				<Item Name="Check Special Tags.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Check Special Tags.vi"/>
				<Item Name="TagReturnType.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/TagReturnType.ctl"/>
				<Item Name="Set String Value.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Set String Value.vi"/>
				<Item Name="GetRTHostConnectedProp.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/GetRTHostConnectedProp.vi"/>
				<Item Name="Error Code Database.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Error Code Database.vi"/>
				<Item Name="Format Message String.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Format Message String.vi"/>
				<Item Name="Find Tag.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Find Tag.vi"/>
				<Item Name="Search and Replace Pattern.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Search and Replace Pattern.vi"/>
				<Item Name="Set Bold Text.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Set Bold Text.vi"/>
				<Item Name="Details Display Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Details Display Dialog.vi"/>
				<Item Name="ErrWarn.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/ErrWarn.ctl"/>
				<Item Name="eventvkey.ctl" Type="VI" URL="/&lt;vilib&gt;/event_ctls.llb/eventvkey.ctl"/>
				<Item Name="Not Found Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Not Found Dialog.vi"/>
				<Item Name="BuildHelpPath.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/BuildHelpPath.vi"/>
				<Item Name="GetHelpDir.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/GetHelpDir.vi"/>
				<Item Name="Get File Extension.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/libraryn.llb/Get File Extension.vi"/>
				<Item Name="EMB_RV_Align.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/uVision/EMB_RV_Align.vi"/>
				<Item Name="LEP_Utilities_GetProjectVISettings.vi" Type="VI" URL="/&lt;vilib&gt;/Platform/EmbProject/utilities/LEP_Utilities_GetProjectVISettings.vi"/>
				<Item Name="Compare Two Paths.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/libraryn.llb/Compare Two Paths.vi"/>
				<Item Name="EMB_RV_Save_Files.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/uVision/EMB_RV_Save_Files.vi"/>
				<Item Name="EMB_RV_Restore_Files.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/uVision/EMB_RV_Restore_Files.vi"/>
				<Item Name="EMB_RV_FileList.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/uVision/EMB_RV_FileList.vi"/>
				<Item Name="LEP_Utilities_GetInfoGlobals.vi" Type="VI" URL="/&lt;vilib&gt;/Platform/EmbProject/Plugins/utilities/LEP_Utilities_GetInfoGlobals.vi"/>
				<Item Name="NI_PackedLibraryUtility.lvlib" Type="Library" URL="/&lt;vilib&gt;/Utility/LVLibp/NI_PackedLibraryUtility.lvlib"/>
				<Item Name="NI_LVConfig.lvlib" Type="Library" URL="/&lt;vilib&gt;/Utility/config.llb/NI_LVConfig.lvlib"/>
				<Item Name="8.6CompatibleGlobalVar.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/8.6CompatibleGlobalVar.vi"/>
				<Item Name="LEP_Utilities_GetVariableManagerIP.vi" Type="VI" URL="/&lt;vilib&gt;/Platform/EmbProject/Plugins/utilities/LEP_Utilities_GetVariableManagerIP.vi"/>
				<Item Name="LEP_Utilities_ResolveIP.vi" Type="VI" URL="/&lt;vilib&gt;/Platform/EmbProject/Plugins/utilities/LEP_Utilities_ResolveIP.vi"/>
				<Item Name="Resolve Machine Alias.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/Resolve Machine Alias.vi"/>
				<Item Name="VariantType.lvlib" Type="Library" URL="/&lt;vilib&gt;/Utility/VariantDataType/VariantType.lvlib"/>
				<Item Name="Emb_Utility_GetBuildSpecId.vi" Type="VI" URL="/&lt;vilib&gt;/Platform/EmbProject/utilities/Emb_Utility_GetBuildSpecId.vi"/>
				<Item Name="TRef TravTarget.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/traverseref.llb/TRef TravTarget.ctl"/>
				<Item Name="TRef Traverse for References.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/traverseref.llb/TRef Traverse for References.vi"/>
				<Item Name="VI Scripting - Traverse.lvlib" Type="Library" URL="/&lt;vilib&gt;/Utility/traverseref.llb/VI Scripting - Traverse.lvlib"/>
				<Item Name="LEP_Utilities_GetCGenErr.vi" Type="VI" URL="/&lt;vilib&gt;/Platform/EmbProject/utilities/LEP_Utilities_GetCGenErr.vi"/>
				<Item Name="Get String Text Bounds.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Get String Text Bounds.vi"/>
				<Item Name="LVBoundsTypeDef.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/miscctls.llb/LVBoundsTypeDef.ctl"/>
				<Item Name="MPU_ASWServer_IsRunningOnAutoTest.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/ASWServer/MPU_ASWServer_IsRunningOnAutoTest.vi"/>
				<Item Name="Space Constant.vi" Type="VI" URL="/&lt;vilib&gt;/dlg_ctls.llb/Space Constant.vi"/>
				<Item Name="RendezvousDataCluster.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/rendezvs.llb/RendezvousDataCluster.ctl"/>
				<Item Name="Release Waiting Procs.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/rendezvs.llb/Release Waiting Procs.vi"/>
				<Item Name="LEP_Utilities_GetServiceContext.vi" Type="VI" URL="/&lt;vilib&gt;/Platform/EmbProject/Plugins/utilities/LEP_Utilities_GetServiceContext.vi"/>
				<Item Name="uV Sim Cycle Units.ctl" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/ARM/ASSG/uV Sim Cycle Units.ctl"/>
				<Item Name="MPU_ASWServer_General_SetProgressBar.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/ASWServer/MPU_ASWServer_General_SetProgressBar.vi"/>
				<Item Name="MPU_ASWServer_General_SetProgressBarTitle.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/ASWServer/MPU_ASWServer_General_SetProgressBarTitle.vi"/>
				<Item Name="MPU_ASWServer_ACKMessage.ctl" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/ASWServer/MPU_ASWServer_ACKMessage.ctl"/>
				<Item Name="MPU_ASWServer_Send.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/ASWServer/MPU_ASWServer_Send.vi"/>
				<Item Name="MPU_ASWServer_Commands.ctl" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/ASWServer/MPU_ASWServer_Commands.ctl"/>
				<Item Name="MPU_ASWServer_ServName.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/ASWServer/MPU_ASWServer_ServName.vi"/>
				<Item Name="MPU_ASWServer_CreateDatagram.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/ASWServer/MPU_ASWServer_CreateDatagram.vi"/>
				<Item Name="Emb_Utility_CalculateCRC.vi" Type="VI" URL="/&lt;vilib&gt;/Platform/EmbProject/utilities/Emb_Utility_CalculateCRC.vi"/>
				<Item Name="MPU_ASWServer_ReadACK.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/ASWServer/MPU_ASWServer_ReadACK.vi"/>
				<Item Name="MPU_ASWServer_GetDatagramElements.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/ASWServer/MPU_ASWServer_GetDatagramElements.vi"/>
				<Item Name="MPU_ASWServer_ErrorHandler.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/ASWServer/MPU_ASWServer_ErrorHandler.vi"/>
				<Item Name="MPU_ASWServer_General_SetProgressBarValue.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/ASWServer/MPU_ASWServer_General_SetProgressBarValue.vi"/>
				<Item Name="MPU_ASWServer_General_SetProgressBarSteps.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/ASWServer/MPU_ASWServer_General_SetProgressBarSteps.vi"/>
				<Item Name="MPU_ASWServer_General_AdvanceProgressBar.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/ASWServer/MPU_ASWServer_General_AdvanceProgressBar.vi"/>
				<Item Name="MPU_ASWServer_GeneralIcons.ctl" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/ASWServer/MPU_ASWServer_GeneralIcons.ctl"/>
				<Item Name="MPU_ASWServer_GeneralCtrls.ctl" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/ASWServer/MPU_ASWServer_GeneralCtrls.ctl"/>
				<Item Name="MPU_ASWServer_General_SetCtrl.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/ASWServer/MPU_ASWServer_General_SetCtrl.vi"/>
				<Item Name="ASW_Close.vi" Type="VI" URL="/&lt;vilib&gt;/LabVIEW Targets/Embedded/uVision/DebugMonitor/ASW_Close.vi"/>
				<Item Name="TRef Traverse.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/traverseref.llb/TRef Traverse.vi"/>
			</Item>
			<Item Name="Emb_CCode_Template.vi" Type="VI" URL="../../../../resource/dialog/Embedded/InterruptManager/Interrupt_CGen/Emb_CCode_Template.vi"/>
			<Item Name="Emb_CGen_Interrupt.vi" Type="VI" URL="../../../../resource/dialog/Embedded/InterruptManager/Interrupt_CGen/Emb_CGen_Interrupt.vi"/>
			<Item Name="Emb_GetActiveProjectItem.vi" Type="VI" URL="../../../../resource/Framework/Providers/lvembedded.llb/Emb_GetActiveProjectItem.vi"/>
			<Item Name="lvembedded.mxx" Type="Document" URL="../../../../resource/Framework/Providers/lvembedded.mxx"/>
			<Item Name="Emb_ActiveProjectItemType.ctl" Type="VI" URL="../../../../resource/Framework/Providers/lvembedded.llb/Emb_ActiveProjectItemType.ctl"/>
			<Item Name="Emb_Append_Enable.vi" Type="VI" URL="../../../../resource/dialog/Embedded/InterruptManager/Interrupt_CGen/Emb_Append_Enable.vi"/>
			<Item Name="Emb_Append_Header.vi" Type="VI" URL="../../../../resource/dialog/Embedded/InterruptManager/Interrupt_CGen/Emb_Append_Header.vi"/>
			<Item Name="Emb_Append_Initialization.vi" Type="VI" URL="../../../../resource/dialog/Embedded/InterruptManager/Interrupt_CGen/Emb_Append_Initialization.vi"/>
			<Item Name="Emb_Append_Registration.vi" Type="VI" URL="../../../../resource/dialog/Embedded/InterruptManager/Interrupt_CGen/Emb_Append_Registration.vi"/>
			<Item Name="Emb_CGen_Interrupt_Template.vi" Type="VI" URL="../../../../resource/dialog/Embedded/InterruptManager/Interrupt_CGen/Emb_CGen_Interrupt_Template.vi"/>
			<Item Name="Emb_CGen_WriteFile.vi" Type="VI" URL="../../../../resource/dialog/Embedded/InterruptManager/Interrupt_CGen/Emb_CGen_WriteFile.vi"/>
			<Item Name="Emb_CheckInterruptConfig.vi" Type="VI" URL="../../../../resource/dialog/Embedded/InterruptManager/Emb_CheckInterruptConfig.vi"/>
			<Item Name="Emb_Flatten_Params.vi" Type="VI" URL="../../../../resource/dialog/Embedded/InterruptManager/Emb_Flatten_Params.vi"/>
			<Item Name="Emb_Get_Interrupt_Info.vi" Type="VI" URL="../../../../resource/dialog/Embedded/InterruptManager/Emb_Get_Interrupt_Info.vi"/>
			<Item Name="Emb_Get_Interrupt_Path.vi" Type="VI" URL="../../../../resource/dialog/Embedded/InterruptManager/Emb_Get_Interrupt_Path.vi"/>
			<Item Name="Emb_GetRefProperty.vi" Type="VI" URL="../../../../resource/Framework/Providers/lvembedded.llb/Emb_GetRefProperty.vi"/>
			<Item Name="Emb_GetCCallContext.vi" Type="VI" URL="../../../../resource/Framework/Providers/lvembedded.llb/Emb_GetCCallContext.vi"/>
			<Item Name="Emb_GetProjectVIs.vi" Type="VI" URL="../../../../resource/Framework/Providers/Builds/Embedded/Emb_GetProjectVIs.vi"/>
			<Item Name="Emb_GetIntProperty.vi" Type="VI" URL="../../../../resource/Framework/Providers/lvembedded.llb/Emb_GetIntProperty.vi"/>
			<Item Name="Emb_GetTargetPath.vi" Type="VI" URL="../../../../resource/dialog/Embedded/InterruptManager/Emb_GetTargetPath.vi"/>
			<Item Name="Emb_Interrupt_FuncsUsed.vi" Type="VI" URL="../../../../resource/dialog/Embedded/InterruptManager/Interrupt_CGen/Emb_Interrupt_FuncsUsed.vi"/>
			<Item Name="optionsFrame_EventData1.ctl" Type="VI" URL="../../../../resource/dialog/PreferencesDialog/optionsFrame_EventData1.ctl"/>
			<Item Name="optionsFrame_PageInitData.ctl" Type="VI" URL="../../../../resource/dialog/PreferencesDialog/optionsFrame_PageInitData.ctl"/>
			<Item Name="optionsFrame_GetPageReadyNotifier.vi" Type="VI" URL="../../../../resource/dialog/PreferencesDialog/optionsFrame_GetPageReadyNotifier.vi"/>
			<Item Name="optionsFrame_GetErrorReportQueue.vi" Type="VI" URL="../../../../resource/dialog/PreferencesDialog/optionsFrame_GetErrorReportQueue.vi"/>
			<Item Name="optionsFrame_ErrorInfo.ctl" Type="VI" URL="../../../../resource/dialog/PreferencesDialog/optionsFrame_ErrorInfo.ctl"/>
			<Item Name="Emb_Get_Provided_Interrupt_Data.vi" Type="VI" URL="../../../../resource/dialog/Embedded/InterruptManager/Emb_Get_Provided_Interrupt_Data.vi"/>
			<Item Name="Emb_RW_Read_Interrupt_Info.vi" Type="VI" URL="../../../../resource/dialog/Embedded/InterruptManager/Emb_RW_Read_Interrupt_Info.vi"/>
			<Item Name="BUIP_ExecuteExplore.vi" Type="VI" URL="../../../../resource/Framework/Providers/Builds/Common/BUIP_ExecuteExplore.vi"/>
			<Item Name="Emb_Get_Files.vi" Type="VI" URL="../../../../resource/dialog/Embedded/InterruptManager/Interrupt_CGen/Emb_Get_Files.vi"/>
			<Item Name="Emb_GUID_To_Path.vi" Type="VI" URL="../../../../resource/dialog/Embedded/InterruptManager/Emb_GUID_To_Path.vi"/>
			<Item Name="Emb_Init_Defines.vi" Type="VI" URL="../../../../resource/dialog/Embedded/InterruptManager/Interrupt_CGen/Emb_Init_Defines.vi"/>
			<Item Name="provcom_GetTargetOS.vi" Type="VI" URL="../../../../resource/Framework/Providers/Common/provcom_GetTargetOS.vi"/>
			<Item Name="provcom_StringGlobals.vi" Type="VI" URL="../../../../resource/Framework/Providers/Common/provcom_StringGlobals.vi"/>
			<Item Name="UVSock_REF_Inc.vi" Type="VI" URL="../common/UVSock/UVSock_REF_Inc.vi"/>
			<Item Name="UVSock_REF_Dec.vi" Type="VI" URL="../common/UVSock/UVSock_REF_Dec.vi"/>
			<Item Name="UVSock_REF_Count.vi" Type="VI" URL="../common/UVSock/UVSock_REF_Count.vi"/>
			<Item Name="UVSock_DecodeBPReason.vi" Type="VI" URL="../common/UVSock/implementation/DataEncodeDecode/UVSock_DecodeBPReason.vi"/>
			<Item Name="Emb_GetStrProperty.vi" Type="VI" URL="../../../../resource/Framework/Providers/lvembedded.llb/Emb_GetStrProperty.vi"/>
			<Item Name="Emb_GetBoolProperty.vi" Type="VI" URL="../../../../resource/Framework/Providers/lvembedded.llb/Emb_GetBoolProperty.vi"/>
			<Item Name="Emb_InitFunclistVIs.vi" Type="VI" URL="../../../../resource/Framework/Providers/Builds/Embedded/Emb_InitFunclistVIs.vi"/>
			<Item Name="Emb_UninitFunclistVIs.vi" Type="VI" URL="../../../../resource/Framework/Providers/Builds/Embedded/Emb_UninitFunclistVIs.vi"/>
			<Item Name="Emb_GetClonableVIs.vi" Type="VI" URL="../../../../resource/Framework/Providers/Builds/Embedded/Emb_GetClonableVIs.vi"/>
			<Item Name="Emb_IntDef_Template.vi" Type="VI" URL="../../../../resource/dialog/Embedded/InterruptManager/Interrupt_CGen/Emb_IntDef_Template.vi"/>
			<Item Name="Emb_IsFunclistVI.vi" Type="VI" URL="../../../../resource/Framework/Providers/Builds/Embedded/Emb_IsFunclistVI.vi"/>
			<Item Name="Emb_Optimize_Clone.vi" Type="VI" URL="../../../../resource/Framework/Providers/Builds/Embedded/Emb_Optimize_Clone.vi"/>
			<Item Name="Emb_Provided_Interrupt.ctl" Type="VI" URL="../../../../resource/dialog/Embedded/InterruptManager/Emb_Provided_Interrupt.ctl"/>
			<Item Name="Emb_Read_Interrupt_XML.vi" Type="VI" URL="../../../../resource/dialog/Embedded/InterruptManager/Emb_Read_Interrupt_XML.vi"/>
			<Item Name="Emb_Read_and_Write_VISettings_Info.vi" Type="VI" URL="../../../../resource/Framework/Providers/Builds/Embedded/Emb_Read_and_Write_VISettings_Info.vi"/>
			<Item Name="Emb_VISettings_Info.ctl" Type="VI" URL="../../../../resource/Framework/Providers/Builds/Embedded/Emb_VISettings_Info.ctl"/>
			<Item Name="Emb_VISettings.ctl" Type="VI" URL="../../../../resource/Framework/Providers/Builds/Embedded/Emb_VISettings.ctl"/>
			<Item Name="Emb_VISetting.ctl" Type="VI" URL="../../../../resource/Framework/Providers/Builds/Embedded/Emb_VISetting.ctl"/>
			<Item Name="Emb_RW_Read_VISettings_Info.vi" Type="VI" URL="../../../../resource/Framework/Providers/Builds/Embedded/Emb_RW_Read_VISettings_Info.vi"/>
			<Item Name="Emb_RW_Write_VISettings_Info.vi" Type="VI" URL="../../../../resource/Framework/Providers/Builds/Embedded/Emb_RW_Write_VISettings_Info.vi"/>
			<Item Name="Emb_SetIntProperty.vi" Type="VI" URL="../../../../resource/Framework/Providers/lvembedded.llb/Emb_SetIntProperty.vi"/>
			<Item Name="Emb_SetStrProperty.vi" Type="VI" URL="../../../../resource/Framework/Providers/lvembedded.llb/Emb_SetStrProperty.vi"/>
			<Item Name="Emb_SetRefProperty.vi" Type="VI" URL="../../../../resource/Framework/Providers/lvembedded.llb/Emb_SetRefProperty.vi"/>
			<Item Name="Emb_Used_Interrupt.ctl" Type="VI" URL="../../../../resource/dialog/Embedded/InterruptManager/Emb_Used_Interrupt.ctl"/>
			<Item Name="Emb_Validate_Interrupt_List.vi" Type="VI" URL="../../../../resource/dialog/Embedded/InterruptManager/Emb_Validate_Interrupt_List.vi"/>
			<Item Name="Emb_VISettings_2DArray.vi" Type="VI" URL="../../../../resource/Framework/Providers/Builds/Embedded/Emb_VISettings_2DArray.vi"/>
			<Item Name="LEP_RV_CGen_GetTargetName.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_CGen_GetTargetName.vi"/>
			<Item Name="Emb_Thread_Handlers.vi" Type="VI" URL="../../../../resource/dialog/Embedded/InterruptManager/Interrupt_CGen/Emb_Thread_Handlers.vi"/>
			<Item Name="Emb_Thread_Wrapper.vi" Type="VI" URL="../../../../resource/dialog/Embedded/InterruptManager/Interrupt_CGen/Emb_Thread_Wrapper.vi"/>
			<Item Name="Emb_VISetting2.ctl" Type="VI" URL="../../../../resource/Framework/Providers/Builds/Embedded/Emb_VISetting2.ctl"/>
			<Item Name="ISR_MutateXML.vi" Type="VI" URL="../../../../resource/dialog/Embedded/InterruptEditor/InterruptCreator/ISR_MutateXML.vi"/>
			<Item Name="LEP_RV_CheckDebug.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_CheckDebug.vi"/>
			<Item Name="LEP_RV_Check_uV4.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_Check_uV4.vi"/>
			<Item Name="LEP_RV_Debug_Daemon_GetServiceContext.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_Debug_Daemon_GetServiceContext.vi"/>
			<Item Name="LEP_RV_GenerateCode.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_GenerateCode.vi"/>
			<Item Name="LEP_RV_GetTCPDebugHostIP.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_GetTCPDebugHostIP.vi"/>
			<Item Name="LEP_RV_GetTCPDefaultIP.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_GetTCPDefaultIP.vi"/>
			<Item Name="LEP_RV_IncBuild_Clear.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_IncBuild_Clear.vi"/>
			<Item Name="LEP_RV_IncBuild_Read.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_IncBuild_Read.vi"/>
			<Item Name="LEP_RV_IsLaunching.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_IsLaunching.vi"/>
			<Item Name="LEP_RV_LaunchGlobals.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_LaunchGlobals.vi"/>
			<Item Name="LEP_RV_LaunchLoadConfig.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_LaunchLoadConfig.vi"/>
			<Item Name="LEP_RV_MemoryMap_TCP.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_MemoryMap_TCP.vi"/>
			<Item Name="LEP_RV_PatchVISettings.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_PatchVISettings.vi"/>
			<Item Name="LEP_RV_UVSetCrossModuleOptimization.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_UVSetCrossModuleOptimization.vi"/>
			<Item Name="LEP_RV_UVSetMicroLib.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_UVSetMicroLib.vi"/>
			<Item Name="UVSock_FilterBuildOutput.vi" Type="VI" URL="../common/UVSock/UVSock_FilterBuildOutput.vi"/>
			<Item Name="LEP_RV_GetSubTargetPath.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_GetSubTargetPath.vi"/>
			<Item Name="LEP_RV_SanityCheck.vi" Type="VI" URL="../RealView/RV_LEP_TargetPlugin/LEP_RV_SanityCheck.vi"/>
			<Item Name="UVSock_PRJ_UPDATE_GROUP.vi" Type="VI" URL="../common/UVSock/UVSock_PRJ_UPDATE_GROUP.vi"/>
		</Item>
		<Item Name="Build Specifications" Type="Build"/>
	</Item>
</Project>
