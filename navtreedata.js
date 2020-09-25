/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "The Sparta Modeling Framework", "index.html", [
    [ "Sparta", "index.html", null ],
    [ "Analysis Tools", "tools.html", null ],
    [ "Simulator Application Level", "application.html", null ],
    [ "Best Practices, Suggestions on using Sparta", "best_practices.html", [
      [ "Using Sparta Asserts", "best_practices.html#sparta_asserts", null ],
      [ "Using Sparta compile-time hints", "best_practices.html#sparta_expect", null ],
      [ "Take Advantage of C++11/17!", "best_practices.html#cpp_eleven", null ],
      [ "How to Use the Port/Event Mechansim Effectively", "best_practices.html#discrete_event", [
        [ "Phases", "best_practices.html#phases", null ],
        [ "Using The Phases", "best_practices.html#using_phase", null ]
      ] ]
    ] ],
    [ "Building the Framework", "building.html", null ],
    [ "Sparta Client APIs", "client_apis.html", null ],
    [ "Command Command Line Interface", "common_cmdline.html", [
      [ "CommandLineSimulator", "common_cmdline.html#cmdline_sim", [
        [ "Sparta Header Files", "building.html#includes", null ],
        [ "Sparta Libraries", "building.html#linking", null ],
        [ "Logging Configuration", "common_cmdline.html#cmdline_log_cfg", null ],
        [ "Command Line Simulation Configuration", "common_cmdline.html#cmdline_sim_cfg", null ]
      ] ],
      [ "Example Output", "common_cmdline.html#example_cmdline_help_out", null ]
    ] ],
    [ "Communication, Events, and Scheduling", "communication.html", [
      [ "Ports", "communication.html#comm_ports", [
        [ "sparta::SyncPorts", "communication.html#sync_ports", null ]
      ] ],
      [ "Events", "communication.html#comm_events", null ],
      [ "Clocks", "communication.html#comm_clocks", null ],
      [ "Scheduling", "communication.html#comm_schedulers", [
        [ "sparta::Scheduler", "communication.html#sparta_Scheduler", null ],
        [ "sparta::SysCSpartaSchedulerAdapter", "communication.html#sysc_scheduler", null ]
      ] ]
    ] ],
    [ "Core Example Using Sparta", "core_example.html", [
      [ "Example Core Layout", "core_example.html#example_core", null ],
      [ "Building", "core_example.html#Building", null ],
      [ "Invocations", "core_example.html#Invocations", [
        [ "Running/Configuring", "core_example.html#run_config_core_example", null ],
        [ "Debugging/Logging", "core_example.html#debugging_logging_core_example", null ],
        [ "Generating Reports", "core_example.html#reporting_core_example", null ],
        [ "Generating Pipeouts", "core_example.html#pipeouts_core_example", null ]
      ] ]
    ] ],
    [ "Sparta Command Line Interface End-User Guide", "end_user.html", [
      [ "1 Simulator Invocation", "end_user.html#invocation", [
        [ "1.1 I/O Policies", "end_user.html#io_policies", null ],
        [ "1.2 Sparta Basic Command-Line Options", "end_user.html#sparta_cmds", null ],
        [ "1.3 Application-Specific Commands", "end_user.html#app_cmds", null ],
        [ "1.4 Sparta Advanced commands", "end_user.html#sparta_advanced_cmds", null ],
        [ "1.5 Sparta Simulation Debug commands", "end_user.html#sim_dbg_cmds", null ]
      ] ],
      [ "2 Control and Configuration", "end_user.html#ctrl_cfg", [
        [ "2.1 Parameters", "end_user.html#ctrl_cfg_parameters", null ],
        [ "2.2 Selecting Architectures", "end_user.html#ctrl_cfg_architecture", null ],
        [ "2.3 Numeric Constants", "end_user.html#numeric_constants", null ]
      ] ],
      [ "3 Simulator Output", "end_user.html#output", [
        [ "3.1 Automatic Summary", "end_user.html#auto_summary", null ],
        [ "3.2 Report Generation", "end_user.html#report_gen", null ],
        [ "3.3 Message Logging", "end_user.html#msg_logging", null ],
        [ "3.4 Notification Logging", "end_user.html#notification_logging", null ],
        [ "3.5 Performance Events", "end_user.html#perf_events", null ],
        [ "3.6 Pipeline Collection", "end_user.html#pipeline_collection", null ],
        [ "3.7 Post-Run Debug dumps", "end_user.html#debug_dump", null ],
        [ "3.4 Backtraces", "end_user.html#backtraces", null ],
        [ "3.8 Device Tree Inspection", "end_user.html#tree_inspection", null ]
      ] ],
      [ "4 Running with a debugger", "end_user.html#run_with_debugger", [
        [ "GDB", "end_user.html#run_with_dbg_gdb", null ],
        [ "Other Debuggers", "end_user.html#run_with_dbg_other", null ]
      ] ],
      [ "4 Post-processing and Visualization", "end_user.html#data_proc_vis", [
        [ "4.1 Pipeline viewer (Argos)", "end_user.html#argos", null ]
      ] ]
    ] ],
    [ "Sparta File formats", "formats.html", null ],
    [ "Parameter/Configuration Format (.cfg,.yaml)", "param_format.html", [
      [ "Nesting", "param_format.html#param_format_nesting", null ],
      [ "Parameter Assignment Attributes and Optional Parameters", "param_format.html#param_format_attributes", null ],
      [ "Examples", "param_format.html#param_format_examples", null ]
    ] ],
    [ "Report Definition Format (.rrep,.yaml)", "report_def_format.html", [
      [ "Overview", "report_def_format.html#report_def_overview", null ],
      [ "Implementation", "report_def_format.html#report_def", null ],
      [ "Structure", "report_def_format.html#report_def_structure", [
        [ "Report Fields", "report_def_format.html#report_def_field", null ],
        [ "Example Report Definition", "report_def_format.html#report_def_example", null ],
        [ "Field Declarations", "report_def_format.html#report_def_field_declarations", null ],
        [ "Field Name Variables", "report_def_format.html#report_def_field_name_variables", null ],
        [ "Subreports", "report_def_format.html#report_def_subreports", null ],
        [ "Reserved Nodes", "report_def_format.html#report_def_reserved_nodes", null ],
        [ "Node Scope Qualifiers", "report_def_format.html#report_def_scope_qual", null ],
        [ "scope Wildcards", "report_def_format.html#report_def_scope_wildcards", null ],
        [ "Statistical Expressions", "report_def_format.html#report_def_expressions", null ],
        [ "Include Directives", "report_def_format.html#report_def_includes", null ],
        [ "Style section", "report_def_format.html#report_def_style", null ],
        [ "Autopopulation Blocks", "report_def_format.html#report_def_autopop", null ]
      ] ],
      [ "Field Declaration Path/Expression Ambiguity", "report_def_format.html#report_def_var_name_ambiguity", null ],
      [ "Report Definition Directive and Option Reference", "report_def_format.html#report_def_directive_ref", null ],
      [ "Limitations of Report Definitions", "report_def_format.html#report_def_lims", null ]
    ] ],
    [ "Report Output Formats", "report_out_format.html", [
      [ "Plaintext (.txt, .text)", "report_out_format.html#report_out_format_plaintext", null ],
      [ "CSV (.csv)", "report_out_format.html#report_out_format_csv", null ],
      [ "BasicHTML (.html, .htm)", "report_out_format.html#report_out_format_basichtml", null ],
      [ "Gnuplot (.gnuplot, .gplt)", "report_out_format.html#report_out_format_gnuplot", null ],
      [ "PythonDict (.python, .python)", "report_out_format.html#report_out_format_pythondict", null ],
      [ "JavascriptObject (.json)", "report_out_format.html#report_out_format_json", null ]
    ] ],
    [ "Pipeline Collection Format (index.bin, location.dat, record.bin, simulation.info, clock.dat)", "pipeout_format.html", null ],
    [ "Checkpointer Format", "checkpoint_format.html", null ],
    [ "Logging Output formats (.log, .log.raw, .log.basic, log.verbose)", "log_out_format.html", null ],
    [ "Framework Development", "framework_dev.html", [
      [ "Changelogs", "framework_dev.html#changelogs", null ],
      [ "Regression Testing", "framework_dev.html#regression", null ],
      [ "Meta Documentation", "framework_dev.html#meta_doc", null ],
      [ "Page Content", "framework_dev.html#content", [
        [ "Feature Description", "framework_dev.html#desc", null ],
        [ "Goals", "framework_dev.html#goal", null ],
        [ "Relevant Requirements", "framework_dev.html#reqs", null ],
        [ "Conceptual Usage", "framework_dev.html#concept", null ],
        [ "Example Usage", "framework_dev.html#examples", null ],
        [ "References", "framework_dev.html#references", null ]
      ] ],
      [ "convention Doxygen Convention", "framework_dev.html#doxygen", [
        [ "Code Documentation Convention", "framework_dev.html#code_conv", null ],
        [ "Texual Documentation Convention", "framework_dev.html#text_conv", null ]
      ] ]
    ] ],
    [ "Sparta Kernel Interface APIs (sparta/kernel)", "kernel_overview.html", null ],
    [ "Textual Message Logging", "logging.html", [
      [ "Logging System Goals", "logging.html#logging_goals", null ],
      [ "Logging System Design Requirements", "logging.html#log_des_reqs", null ],
      [ "Conceptual Usage", "logging.html#logg_concept_usage", [
        [ "Scoped Logging", "logging.html#scoped_log", null ],
        [ "Global Logging", "logging.html#global_log", null ]
      ] ],
      [ "Capturing Log Messages", "logging.html#log_capturing", null ],
      [ "Usage Notes", "logging.html#log_usage_notes", null ],
      [ "Implementation Notes", "logging.html#log_impl_notes", null ]
    ] ],
    [ "Sparta API", "modeling.html", null ],
    [ "Simulator Configuration", "config.html", [
      [ "Configuration Goals", "config.html#config_goals", null ],
      [ "Simulator Subclass Configuration", "config.html#config_usage", null ],
      [ "Configuration System Design Requirements", "config.html#config_des_req", null ]
    ] ],
    [ "Skeleton Pipeline Using Sparta", "skeleton_example.html", [
      [ "Skeleton Pipeline Layout", "skeleton_example.html#skeleton_layout", [
        [ "Sparta Pieces/Parts", "modeling.html#spartaparts", null ],
        [ "Sparta Application APIs (sparta/app)", "modeling.html#app_overview", null ],
        [ "Sparta Events API (sparta/events)", "modeling.html#events_overview", null ],
        [ "Sparta Ports API (sparta/ports)", "modeling.html#ports_overview", null ],
        [ "Sparta Statistics/Counter API (sparta/statistics)", "modeling.html#statistics_overview", null ],
        [ "Sparta Collection APIs (sparta/collection)", "modeling.html#collection_overview", null ],
        [ "Sparta for Functional Modeling (sparta/functional)", "modeling.html#functional_parts", null ],
        [ "Sparta Memory Interface APIs (sparta/memory)", "modeling.html#memory_overview", null ],
        [ "Sparta Utilities/Tools APIs (sparta/utils)", "modeling.html#utils_overview", null ],
        [ "Looking at the Code", "skeleton_example.html#skeleton_looking", null ]
      ] ]
    ] ],
    [ "SystemC Models", "systemc.html", [
      [ "Building", "systemc.html#building_sysc", null ],
      [ "SystemC/Sparta Adapter", "systemc.html#sysc_adapter", null ]
    ] ],
    [ "Timed Component Tutorial", "perf_model_tutorial.html", null ],
    [ "Precedence operators for EventNode/Scheduleables", "precedence_rules.html", null ],
    [ "RegisterProxy Usage", "RegisterProxy_Usage.html", null ],
    [ "Todo List", "todo.html", null ],
    [ "Deprecated List", "deprecated.html", null ],
    [ "Namespaces", "namespaces.html", [
      [ "Namespace List", "namespaces.html", "namespaces_dup" ],
      [ "Namespace Members", "namespacemembers.html", [
        [ "All", "namespacemembers.html", null ],
        [ "Functions", "namespacemembers_func.html", null ],
        [ "Variables", "namespacemembers_vars.html", null ],
        [ "Typedefs", "namespacemembers_type.html", null ],
        [ "Enumerations", "namespacemembers_enum.html", null ],
        [ "Enumerator", "namespacemembers_eval.html", null ]
      ] ]
    ] ],
    [ "Classes", "annotated.html", [
      [ "Class List", "annotated.html", "annotated_dup" ],
      [ "Class Index", "classes.html", null ],
      [ "Class Hierarchy", "hierarchy.html", "hierarchy" ],
      [ "Class Members", "functions.html", [
        [ "All", "functions.html", "functions_dup" ],
        [ "Functions", "functions_func.html", "functions_func" ],
        [ "Variables", "functions_vars.html", "functions_vars" ],
        [ "Typedefs", "functions_type.html", null ],
        [ "Enumerations", "functions_enum.html", null ],
        [ "Enumerator", "functions_eval.html", null ],
        [ "Related Functions", "functions_rela.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "File Members", "globals.html", [
        [ "All", "globals.html", null ],
        [ "Functions", "globals_func.html", null ],
        [ "Enumerations", "globals_enum.html", null ],
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"AddressTypes_8hpp_source.html",
"FeatureConfiguration_8hpp.html#a8b630eca4428860a466a06001a876758",
"SimpleMemoryMap_8hpp_source.html",
"classboost_1_1program__options_1_1basic__parsed__options.html",
"classsparta_1_1ArchData.html#a0ed479c0476f48f3cef1facafa2dbc65",
"classsparta_1_1Bus.html#a68e1779862948961934bad62fa9c97de",
"classsparta_1_1DataContainer.html#a02ea13cd94714cf4438fddf021ade16b",
"classsparta_1_1InstrumentationNode.html#a188dda470c66d66dfcc439f72f6388cb",
"classsparta_1_1ParameterBase.html#a0663f6e558287f2d89b8d573504d9eb9",
"classsparta_1_1PhasedPayloadEvent.html",
"classsparta_1_1Queue_1_1QueueIterator.html#abd984385b3f33839ca7fc31d93e52ee2",
"classsparta_1_1ResourceFactoryBase.html#acbd87d92b9fda9e36bfb648811ceb0fc",
"classsparta_1_1SingleCycleUniqueEvent.html#a4f2783e0f57e246cbb03f73f8c151cf9",
"classsparta_1_1StatisticSet.html#ada89ee99a6c73e078029101b98239e04",
"classsparta_1_1TreeNode.html#a8903d008ff279e8348d064e1619c9303",
"classsparta_1_1app_1_1CommandLineSimulator.html#a83e63bc0bd80afa0ed48a995dcb90b05",
"classsparta_1_1app_1_1Simulation.html#a81b99477fa7a93af2839f755bfc9eb4b",
"classsparta_1_1collection_1_1Collectable_3_01DataT_00_01collection__phase_00_01MetaStruct_1_1ena95f85bb7619f65143e3ce37fb66ff5fe.html#a8e4200f604a74bb842b910c5e78948a7",
"classsparta_1_1memory_1_1DMI__DEPRECATED.html",
"classsparta_1_1statistics_1_1StatisticSnapshot.html",
"classsparta_1_1utils_1_1Rational.html#a53e71c7bdbc5839ae37c57d4ef3bdc6a",
"globals_enum.html",
"structMetaStruct_1_1return__type_3_01std_1_1function_3_01R_07Ts_8_8_8_08_4_01const_01T_1_1_5_01_4.html",
"structsparta_1_1statistics_1_1expression_1_1Operation.html#a1602dd1eabbb8917ca850dbeeb0be0a9"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';