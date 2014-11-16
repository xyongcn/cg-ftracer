#!/system/bin/sh
echo 500000 > /d/tracing/buffer_size_kb
echo counter_get_cntvct_cp15 >> /d/tracing/set_ftrace_notrace
echo deflate* >> /d/tracing/set_ftrace_notrace
echo function_graph > /d/tracing/current_tracer
echo 1 > /d/tracing/tracing_on
#cat /d/tracing/trace_pipe
cat /d/tracing/per_cpu/cpu0/trace_pipe
echo "Done ftrace!"
