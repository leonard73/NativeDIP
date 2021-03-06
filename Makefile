#libFormat Path
libNativeDIP_PATH:=DipNative
libDrivers_PATH:=Drivers
example_PATH:=Examples
lib3rd_PATH:=lib3rdParty
cc:=gcc
#examples sources#
ex_benchmark_src:=${example_PATH}/benchmark.c
#drivers sources#
driver_src:=${libDrivers_PATH}/sources/opengl/*.c \
			${libDrivers_PATH}/sources/uvcFrame/*.c \
			${libDrivers_PATH}/sources/format/*.c
#dip native sources#
dipNative_src:=${libNativeDIP_PATH}/sources/ISP/*.c
dipNative_src+=${libNativeDIP_PATH}/sources/CONV/*.c
#3rd  party source
lib3rd_src:=${lib3rd_PATH}/*.a
#total sources#
ex_benchmark_src_all:=${ex_benchmark_src} \
					  ${driver_src} \
					  ${dipNative_src}\
					  ${lib3rd_src}
#incldue compile flag
cc_flag_include:=-I${libNativeDIP_PATH}/include \
				 -I${libDrivers_PATH}/include \
				 -I${lib3rd_PATH}/include
#libraries compile flag
cc_flag_lib:=-lGL -lGLU -lglut -lm  -lpthread 
#definiations compile flag
cc_flag_def+=-DDEBUG_LATENCY_ENABLE
cc_flag_def+=-D__DISABLE_LOG_DEBUG__ 
# cc_flag_def+=-D__DISABLE_PIPELINE_LATENCY__
#simd  optimization compile  flag
cc_flag_isa_neon:= -mfpu=neon-vfpv4 -mfloat-abi=hard -O3
cc_flag_isa_avx:=  -mavx  -O3
cc_flag_isa_avx2:= -mavx2 -O3
cc_flag_isa_avx512:= -mavx512f -O3
#choose which platform your device use
# cc_flag_isa:=${cc_flag_isa_avx}
# cc_flag_isa:=${cc_flag_isa_avx2}
# cc_flag_isa:=${cc_flag_isa_neon}
#compile flag all
cc_flag_all:=${cc_flag_lib} ${cc_flag_include} ${cc_flag_def} ${cc_flag_isa}
#outExecutable
exefile:=build/DipBenchmark
build_prepare:
		mkdir -p ./build
build_nativeC:
		make build_prepare
		$(cc) -o $(exefile) $(ex_benchmark_src_all) $(cc_flag_all)
build_armNeon:
		make build_prepare
		$(cc) -o $(exefile) $(ex_benchmark_src_all) $(cc_flag_all) ${cc_flag_isa_neon}
build_avx128:
		make build_prepare
		$(cc) -o $(exefile) $(ex_benchmark_src_all) $(cc_flag_all) ${cc_flag_isa_avx}
build_avx256:
		make build_prepare
		$(cc) -o $(exefile) $(ex_benchmark_src_all) $(cc_flag_all) ${cc_flag_isa_avx2}
build_avx512:
		make build_prepare
		$(cc) -o $(exefile) $(ex_benchmark_src_all) $(cc_flag_all) ${cc_flag_isa_avx512}
run_bmp_display:
		./$(exefile) ${exe_parameters} 1 0 4
run_uvc_display:
		./$(exefile) ${exe_parameters} 2 2 4
clean:
		rm -rf $(exefile)
all:
		make clean
		make build_nativeC
		make run_bmp_display