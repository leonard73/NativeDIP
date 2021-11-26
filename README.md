【INTRODUCTION】

This is an open source project of Digital Image Processing Algorithm with Native C language , which could be built both on X86/X64 and ARM-LINUX platform.

【BUILDING】

1.sh intall.sh => to install necessary libraries on linux os

2.options on your platform:

 [2.1] sh build_nativeC.sh   =>  build projects with no platform dependencies for it will build native c language instead of AVX/SSE/NEON ...
 
 [2.2] sh build_AVX128.sh    =>  build projects on X86/X64 AVX      supported Linux OS with AVX128 SIMD Optimizations

 [2.3] sh build_AVX256.sh    =>  build projects on X86/X64 AVX2     supported Linux OS with AVX256 SIMD Optimizations

 [2.4] sh build_AVX512.sh    =>  build projects on X86/X64 AVX3     supported Linux OS with AVX512 SIMD Optimizations

 [2.5] sh build_armNeon.sh   =>  build projects on arm Neon         supported Linux OS with NEON   SIMD Optimizations

【RUNNING】

3.choices to run the executable files

 [3.1] sh run_bmp_benchmark.sh  =>   run demos with the input and output of target bmp files
 
 [3.2] sh run_uvc_benchmark.sh  =>   run demos with the uvc camera input (Need Set VideoX number inside the driver) and display with opengl in real time.


