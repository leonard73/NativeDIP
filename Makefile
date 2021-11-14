#libFormat Path
libNativeDIP_PATH:=DipNative
libDrivers_PATH:=Drivers
example_PATH:=Examples
cc:=gcc
#examples sources#
ex_benchmark_src:=${example_PATH}/benchmark.c
#drivers sources#
driver_src:=${libDrivers_PATH}/sources/opengl/*.c \
			${libDrivers_PATH}/sources/uvcFrame/*.c \
			${libDrivers_PATH}/sources/format/*.c
#dip native sources#
dipNative_src:=${libNativeDIP_PATH}/sources/ISP/*.c
#total sources#
ex_benchmark_src_all:=${ex_benchmark_src} \
					  ${driver_src} \
					  ${dipNative_src}	
#incldue compile flag
cc_flag_include:=-I${libNativeDIP_PATH}/include \
				 -I${libDrivers_PATH}/include
#libraries compile flag
cc_flag_lib:=-lGL -lGLU -lglut
#definiations compile flag
# cc_flag_def:=-DDEBUG_LATENCY_ENABLE 
#simd  optimization compile  flag
cc_flag_isa_neon:= -mfpu=neon-vfpv4 -mfloat-abi=hard -O3
cc_flag_isa_avx:=  -mavx  -O3
cc_flag_isa_avx2:= -mavx2 -O3
#choose which platform your device use
# cc_flag_isa:=${cc_flag_isa_avx}
# cc_flag_isa:=${cc_flag_isa_avx2}
# cc_flag_isa:=${cc_flag_isa_neon}
#compile flag all
cc_flag_all:=${cc_flag_lib} ${cc_flag_include} ${cc_flag_def} ${cc_flag_isa}
#outExecutable
exefile:=DipBenchmark
build:
		$(cc) -o $(exefile) $(ex_benchmark_src_all) $(cc_flag_all)
run_bmp_display:
		./$(exefile) ${exe_parameters} 1
run_uvc_display:
		./$(exefile) ${exe_parameters} 2
clean:
		rm -rf $(exefile)
all:
		make clean
		make build
		make run_bmp_display