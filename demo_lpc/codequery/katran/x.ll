; ModuleID = 'x.c'
source_filename = "x.c"
target datalayout = "e-m:e-p:64:64-i64:64-n32:64-S128"
target triple = "bpf"

%struct.anon = type { [6 x i32]*, i32*, %struct.decap_stats*, [1024 x i32]*, [0 x i32]* }
%struct.decap_stats = type { i64, i64, i64 }
%struct.anon.0 = type { [2 x i32]*, i32*, %struct.ctl_value*, [16 x i32]*, [0 x i32]* }
%struct.ctl_value = type { %union.anon }
%union.anon = type { i64 }
%struct.xdp_md = type { i32, i32, i32, i32, i32 }
%struct.ethhdr = type { [6 x i8], [6 x i8], i16 }
%struct.packet_description = type { %struct.flow_key, i32, i8, i8 }
%struct.flow_key = type { %union.anon.1, %union.anon.2, %union.anon.3, i8 }
%union.anon.1 = type { [4 x i32] }
%union.anon.2 = type { [4 x i32] }
%union.anon.3 = type { i32 }

@_license = dso_local global [4 x i8] c"GPL\00", section "license", align 1, !dbg !0
@decap_counters = dso_local global %struct.anon zeroinitializer, section ".maps", align 8, !dbg !51
@ctl_array = dso_local global %struct.anon.0 zeroinitializer, section ".maps", align 8, !dbg !83
@llvm.used = appending global [5 x i8*] [i8* getelementptr inbounds ([4 x i8], [4 x i8]* @_license, i32 0, i32 0), i8* bitcast (%struct.anon.0* @ctl_array to i8*), i8* bitcast (%struct.anon* @decap_counters to i8*), i8* bitcast (i32 (%struct.xdp_md*)* @xdp_prog_simple to i8*), i8* bitcast (i32 (%struct.xdp_md*)* @xdpdecap to i8*)], section "llvm.metadata"

; Function Attrs: nounwind
define dso_local i32 @xdpdecap(%struct.xdp_md* %0) #0 section "xdp" !dbg !133 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  call void @llvm.dbg.value(metadata %struct.xdp_md* %0, metadata !146, metadata !DIExpression()), !dbg !162
  %4 = getelementptr inbounds %struct.xdp_md, %struct.xdp_md* %0, i64 0, i32 0, !dbg !163
  %5 = load i32, i32* %4, align 4, !dbg !163, !tbaa !164
  %6 = zext i32 %5 to i64, !dbg !169
  %7 = inttoptr i64 %6 to i8*, !dbg !170
  call void @llvm.dbg.value(metadata i8* %7, metadata !147, metadata !DIExpression()), !dbg !162
  %8 = getelementptr inbounds %struct.xdp_md, %struct.xdp_md* %0, i64 0, i32 1, !dbg !171
  %9 = load i32, i32* %8, align 4, !dbg !171, !tbaa !172
  %10 = zext i32 %9 to i64, !dbg !173
  %11 = inttoptr i64 %10 to i8*, !dbg !174
  call void @llvm.dbg.value(metadata i8* %11, metadata !148, metadata !DIExpression()), !dbg !162
  call void @llvm.dbg.value(metadata i8* %7, metadata !149, metadata !DIExpression()), !dbg !162
  call void @llvm.dbg.value(metadata i32 14, metadata !161, metadata !DIExpression()), !dbg !162
  %12 = getelementptr i8, i8* %7, i64 14, !dbg !175
  %13 = icmp ugt i8* %12, %11, !dbg !177
  br i1 %13, label %113, label %14, !dbg !178

14:                                               ; preds = %1
  %15 = inttoptr i64 %6 to %struct.ethhdr*, !dbg !179
  call void @llvm.dbg.value(metadata %struct.ethhdr* %15, metadata !149, metadata !DIExpression()), !dbg !162
  %16 = getelementptr inbounds %struct.ethhdr, %struct.ethhdr* %15, i64 0, i32 2, !dbg !180
  %17 = load i16, i16* %16, align 1, !dbg !180, !tbaa !181
  call void @llvm.dbg.value(metadata i16 %17, metadata !160, metadata !DIExpression()), !dbg !162
  switch i16 %17, label %113 [
    i16 8, label %18
    i16 -8826, label %70
  ], !dbg !184

18:                                               ; preds = %14
  call void @llvm.dbg.value(metadata i8* %7, metadata !185, metadata !DIExpression()) #3, !dbg !228
  call void @llvm.dbg.value(metadata i64 14, metadata !191, metadata !DIExpression()) #3, !dbg !228
  call void @llvm.dbg.value(metadata i8* %11, metadata !192, metadata !DIExpression()) #3, !dbg !228
  call void @llvm.dbg.value(metadata i1 false, metadata !193, metadata !DIExpression()) #3, !dbg !228
  call void @llvm.dbg.value(metadata %struct.xdp_md* %0, metadata !194, metadata !DIExpression()) #3, !dbg !228
  call void @llvm.dbg.value(metadata i8 0, metadata !195, metadata !DIExpression(DW_OP_LLVM_fragment, 288, 8)) #3, !dbg !228
  %19 = bitcast i32* %3 to i8*, !dbg !232
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %19) #3, !dbg !232
  call void @llvm.dbg.value(metadata i32 0, metadata !224, metadata !DIExpression()) #3, !dbg !228
  store i32 0, i32* %3, align 4, !dbg !233, !tbaa !234
  call void @llvm.dbg.value(metadata i8* %7, metadata !185, metadata !DIExpression()) #3, !dbg !228
  call void @llvm.dbg.value(metadata i8* %11, metadata !192, metadata !DIExpression()) #3, !dbg !228
  call void @llvm.dbg.value(metadata i1 false, metadata !193, metadata !DIExpression()) #3, !dbg !228
  call void @llvm.dbg.value(metadata %struct.packet_description* undef, metadata !235, metadata !DIExpression()) #3, !dbg !296
  call void @llvm.dbg.value(metadata i64 14, metadata !244, metadata !DIExpression()) #3, !dbg !296
  call void @llvm.dbg.value(metadata i8* %7, metadata !246, metadata !DIExpression()) #3, !dbg !296
  call void @llvm.dbg.value(metadata i8* %11, metadata !247, metadata !DIExpression()) #3, !dbg !296
  call void @llvm.dbg.value(metadata i1 false, metadata !248, metadata !DIExpression()) #3, !dbg !296
  call void @llvm.dbg.value(metadata i8* %12, metadata !250, metadata !DIExpression()) #3, !dbg !296
  %20 = getelementptr i8, i8* %7, i64 34, !dbg !298
  %21 = icmp ugt i8* %20, %11, !dbg !302
  br i1 %21, label %68, label %22, !dbg !303

22:                                               ; preds = %18
  %23 = load i8, i8* %12, align 4, !dbg !304
  %24 = and i8 %23, 15, !dbg !304
  %25 = icmp eq i8 %24, 5, !dbg !306
  br i1 %25, label %26, label %68, !dbg !307

26:                                               ; preds = %22
  %27 = getelementptr i8, i8* %7, i64 23, !dbg !308
  %28 = load i8, i8* %27, align 1, !dbg !308, !tbaa !309
  call void @llvm.dbg.value(metadata i8 %28, metadata !195, metadata !DIExpression(DW_OP_LLVM_fragment, 288, 8)) #3, !dbg !228
  call void @llvm.dbg.value(metadata i64 14, metadata !244, metadata !DIExpression(DW_OP_plus_uconst, 20, DW_OP_stack_value)) #3, !dbg !296
  %29 = getelementptr i8, i8* %7, i64 20, !dbg !311
  %30 = bitcast i8* %29 to i16*, !dbg !311
  %31 = load i16, i16* %30, align 2, !dbg !311, !tbaa !313
  %32 = and i16 %31, -193, !dbg !314
  %33 = icmp eq i16 %32, 0, !dbg !314
  br i1 %33, label %34, label %68, !dbg !315

34:                                               ; preds = %26
  call void @llvm.dbg.value(metadata i8 %28, metadata !195, metadata !DIExpression(DW_OP_LLVM_fragment, 288, 8)) #3, !dbg !228
  call void @llvm.dbg.value(metadata i8 %28, metadata !195, metadata !DIExpression(DW_OP_LLVM_fragment, 288, 8)) #3, !dbg !228
  call void @llvm.dbg.value(metadata i32 -1, metadata !226, metadata !DIExpression()) #3, !dbg !228
  call void @llvm.dbg.value(metadata i8 %28, metadata !225, metadata !DIExpression()) #3, !dbg !228
  call void @llvm.dbg.value(metadata i32* %3, metadata !224, metadata !DIExpression(DW_OP_deref)) #3, !dbg !228
  %35 = call i8* inttoptr (i64 1 to i8* (i8*, i8*)*)(i8* bitcast (%struct.anon* @decap_counters to i8*), i8* nonnull %19) #3, !dbg !316
  call void @llvm.dbg.value(metadata i8* %35, metadata !223, metadata !DIExpression()) #3, !dbg !228
  %36 = icmp eq i8* %35, null, !dbg !317
  br i1 %36, label %68, label %37, !dbg !319

37:                                               ; preds = %34
  %38 = getelementptr inbounds i8, i8* %35, i64 16, !dbg !320
  %39 = bitcast i8* %38 to i64*, !dbg !320
  %40 = load i64, i64* %39, align 8, !dbg !321, !tbaa !322
  %41 = add i64 %40, 1, !dbg !321
  store i64 %41, i64* %39, align 8, !dbg !321, !tbaa !322
  call void @llvm.dbg.value(metadata i8 %28, metadata !225, metadata !DIExpression()) #3, !dbg !228
  switch i8 %28, label %68 [
    i8 4, label %42
    i8 41, label %42
  ], !dbg !325

42:                                               ; preds = %37, %37
  call void @llvm.dbg.value(metadata i1 false, metadata !193, metadata !DIExpression()) #3, !dbg !228
  %43 = bitcast i8* %35 to i64*, !dbg !327
  %44 = load i64, i64* %43, align 8, !dbg !331, !tbaa !332
  %45 = add i64 %44, 1, !dbg !331
  store i64 %45, i64* %43, align 8, !dbg !331, !tbaa !332
  call void @llvm.dbg.value(metadata %struct.xdp_md* %0, metadata !333, metadata !DIExpression()) #3, !dbg !347
  call void @llvm.dbg.value(metadata %struct.packet_description* undef, metadata !343, metadata !DIExpression()) #3, !dbg !347
  call void @llvm.dbg.value(metadata i64 undef, metadata !345, metadata !DIExpression()) #3, !dbg !347
  call void @llvm.dbg.value(metadata i16* undef, metadata !346, metadata !DIExpression()) #3, !dbg !347
  switch i8 %28, label %68 [
    i8 4, label %46
    i8 41, label %55
  ], !dbg !349

46:                                               ; preds = %42
  call void @llvm.dbg.value(metadata %struct.xdp_md* %0, metadata !350, metadata !DIExpression()) #3, !dbg !360
  call void @llvm.dbg.value(metadata i8* %7, metadata !359, metadata !DIExpression()) #3, !dbg !360
  call void @llvm.dbg.value(metadata i8* %29, metadata !358, metadata !DIExpression()) #3, !dbg !360
  %47 = getelementptr i8, i8* %7, i64 26, !dbg !367
  %48 = getelementptr inbounds i8, i8* %7, i64 6, !dbg !368
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* nonnull align 1 dereferenceable(6) %47, i8* nonnull align 1 dereferenceable(6) %48, i64 6, i1 false) #3, !dbg !369
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* nonnull align 1 dereferenceable(6) %29, i8* nonnull align 1 dereferenceable(6) %7, i64 6, i1 false) #3, !dbg !370
  %49 = getelementptr i8, i8* %7, i64 32, !dbg !371
  %50 = bitcast i8* %49 to i16*, !dbg !371
  store i16 8, i16* %50, align 1, !dbg !372, !tbaa !181
  %51 = bitcast %struct.xdp_md* %0 to i8*, !dbg !373
  %52 = call i32 inttoptr (i64 44 to i32 (i8*, i32)*)(i8* %51, i32 20) #3, !dbg !375
  %53 = icmp eq i32 %52, 0, !dbg !375
  %54 = select i1 %53, i32 2, i32 1, !dbg !376
  br label %68, !dbg !376

55:                                               ; preds = %42
  %56 = getelementptr i8, i8* %7, i64 54, !dbg !377
  %57 = icmp ugt i8* %56, %11, !dbg !381
  br i1 %57, label %68, label %58, !dbg !382

58:                                               ; preds = %55
  call void @llvm.dbg.value(metadata %struct.xdp_md* %0, metadata !383, metadata !DIExpression()) #3, !dbg !393
  call void @llvm.dbg.value(metadata i1 false, metadata !390, metadata !DIExpression()) #3, !dbg !393
  call void @llvm.dbg.value(metadata i8* %7, metadata !392, metadata !DIExpression()) #3, !dbg !393
  %59 = getelementptr i8, i8* %7, i64 40, !dbg !396
  call void @llvm.dbg.value(metadata i8* %59, metadata !391, metadata !DIExpression()) #3, !dbg !393
  %60 = getelementptr i8, i8* %7, i64 46, !dbg !397
  %61 = getelementptr inbounds i8, i8* %7, i64 6, !dbg !398
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* nonnull align 1 dereferenceable(6) %60, i8* nonnull align 1 dereferenceable(6) %61, i64 6, i1 false) #3, !dbg !399
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* nonnull align 1 dereferenceable(6) %59, i8* nonnull align 1 dereferenceable(6) %7, i64 6, i1 false) #3, !dbg !400
  %62 = getelementptr i8, i8* %7, i64 52, !dbg !401
  %63 = bitcast i8* %62 to i16*, !dbg !401
  store i16 -8826, i16* %63, align 1, !dbg !401, !tbaa !181
  %64 = bitcast %struct.xdp_md* %0 to i8*, !dbg !403
  %65 = call i32 inttoptr (i64 44 to i32 (i8*, i32)*)(i8* %64, i32 40) #3, !dbg !405
  %66 = icmp eq i32 %65, 0, !dbg !405
  %67 = select i1 %66, i32 2, i32 1, !dbg !406
  br label %68, !dbg !406

68:                                               ; preds = %18, %22, %26, %34, %37, %42, %46, %55, %58
  %69 = phi i32 [ 2, %34 ], [ 2, %42 ], [ 2, %37 ], [ 1, %18 ], [ 1, %22 ], [ 1, %26 ], [ 1, %55 ], [ %54, %46 ], [ %67, %58 ], !dbg !228
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %19) #3, !dbg !407
  br label %113, !dbg !408

70:                                               ; preds = %14
  call void @llvm.dbg.value(metadata i8* %7, metadata !185, metadata !DIExpression()) #3, !dbg !409
  call void @llvm.dbg.value(metadata i64 14, metadata !191, metadata !DIExpression()) #3, !dbg !409
  call void @llvm.dbg.value(metadata i8* %11, metadata !192, metadata !DIExpression()) #3, !dbg !409
  call void @llvm.dbg.value(metadata i1 true, metadata !193, metadata !DIExpression()) #3, !dbg !409
  call void @llvm.dbg.value(metadata %struct.xdp_md* %0, metadata !194, metadata !DIExpression()) #3, !dbg !409
  call void @llvm.dbg.value(metadata i8 0, metadata !195, metadata !DIExpression(DW_OP_LLVM_fragment, 288, 8)) #3, !dbg !409
  %71 = bitcast i32* %2 to i8*, !dbg !413
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %71) #3, !dbg !413
  call void @llvm.dbg.value(metadata i32 0, metadata !224, metadata !DIExpression()) #3, !dbg !409
  store i32 0, i32* %2, align 4, !dbg !414, !tbaa !234
  call void @llvm.dbg.value(metadata i8* %7, metadata !185, metadata !DIExpression()) #3, !dbg !409
  call void @llvm.dbg.value(metadata i8* %11, metadata !192, metadata !DIExpression()) #3, !dbg !409
  call void @llvm.dbg.value(metadata i1 true, metadata !193, metadata !DIExpression()) #3, !dbg !409
  call void @llvm.dbg.value(metadata %struct.packet_description* undef, metadata !235, metadata !DIExpression()) #3, !dbg !415
  call void @llvm.dbg.value(metadata i64 14, metadata !244, metadata !DIExpression()) #3, !dbg !415
  call void @llvm.dbg.value(metadata i8* %7, metadata !246, metadata !DIExpression()) #3, !dbg !415
  call void @llvm.dbg.value(metadata i8* %11, metadata !247, metadata !DIExpression()) #3, !dbg !415
  call void @llvm.dbg.value(metadata i1 true, metadata !248, metadata !DIExpression()) #3, !dbg !415
  call void @llvm.dbg.value(metadata i8* %7, metadata !267, metadata !DIExpression(DW_OP_plus_uconst, 14, DW_OP_stack_value)) #3, !dbg !415
  %72 = getelementptr i8, i8* %7, i64 54, !dbg !417
  %73 = icmp ugt i8* %72, %11, !dbg !420
  br i1 %73, label %111, label %74, !dbg !421

74:                                               ; preds = %70
  call void @llvm.dbg.value(metadata i8* %7, metadata !267, metadata !DIExpression(DW_OP_plus_uconst, 14, DW_OP_stack_value)) #3, !dbg !415
  call void @llvm.dbg.value(metadata i64 40, metadata !249, metadata !DIExpression()) #3, !dbg !415
  %75 = getelementptr i8, i8* %7, i64 20, !dbg !422
  %76 = load i8, i8* %75, align 2, !dbg !422, !tbaa !423
  call void @llvm.dbg.value(metadata i8 %76, metadata !195, metadata !DIExpression(DW_OP_LLVM_fragment, 288, 8)) #3, !dbg !409
  call void @llvm.dbg.value(metadata i64 14, metadata !244, metadata !DIExpression(DW_OP_plus_uconst, 40, DW_OP_stack_value)) #3, !dbg !415
  %77 = icmp eq i8 %76, 44, !dbg !426
  br i1 %77, label %111, label %78, !dbg !428

78:                                               ; preds = %74
  call void @llvm.dbg.value(metadata i8 %76, metadata !195, metadata !DIExpression(DW_OP_LLVM_fragment, 288, 8)) #3, !dbg !409
  call void @llvm.dbg.value(metadata i8 %76, metadata !195, metadata !DIExpression(DW_OP_LLVM_fragment, 288, 8)) #3, !dbg !409
  call void @llvm.dbg.value(metadata i32 -1, metadata !226, metadata !DIExpression()) #3, !dbg !409
  call void @llvm.dbg.value(metadata i8 %76, metadata !225, metadata !DIExpression()) #3, !dbg !409
  call void @llvm.dbg.value(metadata i32* %2, metadata !224, metadata !DIExpression(DW_OP_deref)) #3, !dbg !409
  %79 = call i8* inttoptr (i64 1 to i8* (i8*, i8*)*)(i8* bitcast (%struct.anon* @decap_counters to i8*), i8* nonnull %71) #3, !dbg !429
  call void @llvm.dbg.value(metadata i8* %79, metadata !223, metadata !DIExpression()) #3, !dbg !409
  %80 = icmp eq i8* %79, null, !dbg !430
  br i1 %80, label %111, label %81, !dbg !431

81:                                               ; preds = %78
  %82 = getelementptr inbounds i8, i8* %79, i64 16, !dbg !432
  %83 = bitcast i8* %82 to i64*, !dbg !432
  %84 = load i64, i64* %83, align 8, !dbg !433, !tbaa !322
  %85 = add i64 %84, 1, !dbg !433
  store i64 %85, i64* %83, align 8, !dbg !433, !tbaa !322
  call void @llvm.dbg.value(metadata i8 %76, metadata !225, metadata !DIExpression()) #3, !dbg !409
  switch i8 %76, label %111 [
    i8 4, label %86
    i8 41, label %86
  ], !dbg !434

86:                                               ; preds = %81, %81
  call void @llvm.dbg.value(metadata i1 true, metadata !193, metadata !DIExpression()) #3, !dbg !409
  %87 = getelementptr inbounds i8, i8* %79, i64 8, !dbg !435
  %88 = bitcast i8* %87 to i64*, !dbg !435
  %89 = load i64, i64* %88, align 8, !dbg !437, !tbaa !438
  %90 = add i64 %89, 1, !dbg !437
  store i64 %90, i64* %88, align 8, !dbg !437, !tbaa !438
  call void @llvm.dbg.value(metadata %struct.xdp_md* %0, metadata !333, metadata !DIExpression()) #3, !dbg !439
  call void @llvm.dbg.value(metadata %struct.packet_description* undef, metadata !343, metadata !DIExpression()) #3, !dbg !439
  call void @llvm.dbg.value(metadata i64 undef, metadata !345, metadata !DIExpression()) #3, !dbg !439
  call void @llvm.dbg.value(metadata i16* undef, metadata !346, metadata !DIExpression()) #3, !dbg !439
  switch i8 %76, label %111 [
    i8 4, label %91
    i8 41, label %101
  ], !dbg !441

91:                                               ; preds = %86
  call void @llvm.dbg.value(metadata %struct.xdp_md* %0, metadata !383, metadata !DIExpression()) #3, !dbg !442
  call void @llvm.dbg.value(metadata i1 true, metadata !390, metadata !DIExpression()) #3, !dbg !442
  call void @llvm.dbg.value(metadata i8* %7, metadata !392, metadata !DIExpression()) #3, !dbg !442
  %92 = getelementptr i8, i8* %7, i64 40, !dbg !446
  call void @llvm.dbg.value(metadata i8* %92, metadata !391, metadata !DIExpression()) #3, !dbg !442
  %93 = getelementptr i8, i8* %7, i64 46, !dbg !447
  %94 = getelementptr inbounds i8, i8* %7, i64 6, !dbg !448
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* nonnull align 1 dereferenceable(6) %93, i8* nonnull align 1 dereferenceable(6) %94, i64 6, i1 false) #3, !dbg !449
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* nonnull align 1 dereferenceable(6) %92, i8* nonnull align 1 dereferenceable(6) %7, i64 6, i1 false) #3, !dbg !450
  %95 = getelementptr i8, i8* %7, i64 52, !dbg !451
  %96 = bitcast i8* %95 to i16*, !dbg !451
  store i16 8, i16* %96, align 1, !dbg !451, !tbaa !181
  %97 = bitcast %struct.xdp_md* %0 to i8*, !dbg !452
  %98 = call i32 inttoptr (i64 44 to i32 (i8*, i32)*)(i8* %97, i32 40) #3, !dbg !453
  %99 = icmp eq i32 %98, 0, !dbg !453
  %100 = select i1 %99, i32 2, i32 1, !dbg !454
  br label %111, !dbg !454

101:                                              ; preds = %86
  call void @llvm.dbg.value(metadata %struct.xdp_md* %0, metadata !383, metadata !DIExpression()) #3, !dbg !455
  call void @llvm.dbg.value(metadata i1 false, metadata !390, metadata !DIExpression()) #3, !dbg !455
  call void @llvm.dbg.value(metadata i8* %7, metadata !392, metadata !DIExpression()) #3, !dbg !455
  %102 = getelementptr i8, i8* %7, i64 40, !dbg !457
  call void @llvm.dbg.value(metadata i8* %102, metadata !391, metadata !DIExpression()) #3, !dbg !455
  %103 = getelementptr i8, i8* %7, i64 46, !dbg !458
  %104 = getelementptr inbounds i8, i8* %7, i64 6, !dbg !459
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* nonnull align 1 dereferenceable(6) %103, i8* nonnull align 1 dereferenceable(6) %104, i64 6, i1 false) #3, !dbg !460
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* nonnull align 1 dereferenceable(6) %102, i8* nonnull align 1 dereferenceable(6) %7, i64 6, i1 false) #3, !dbg !461
  %105 = getelementptr i8, i8* %7, i64 52, !dbg !462
  %106 = bitcast i8* %105 to i16*, !dbg !462
  store i16 -8826, i16* %106, align 1, !dbg !462, !tbaa !181
  %107 = bitcast %struct.xdp_md* %0 to i8*, !dbg !463
  %108 = call i32 inttoptr (i64 44 to i32 (i8*, i32)*)(i8* %107, i32 40) #3, !dbg !464
  %109 = icmp eq i32 %108, 0, !dbg !464
  %110 = select i1 %109, i32 2, i32 1, !dbg !465
  br label %111, !dbg !465

111:                                              ; preds = %70, %74, %78, %81, %86, %91, %101
  %112 = phi i32 [ 2, %78 ], [ 2, %86 ], [ 2, %81 ], [ 1, %70 ], [ 1, %74 ], [ %100, %91 ], [ %110, %101 ], !dbg !409
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %71) #3, !dbg !466
  br label %113, !dbg !467

113:                                              ; preds = %14, %1, %111, %68
  %114 = phi i32 [ %69, %68 ], [ %112, %111 ], [ 1, %1 ], [ 2, %14 ], !dbg !162
  ret i32 %114, !dbg !468
}

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture) #1

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.lifetime.end.p0i8(i64 immarg, i8* nocapture) #1

; Function Attrs: nounwind
define dso_local i32 @xdp_prog_simple(%struct.xdp_md* %0) #0 section "xdp" !dbg !469 {
  call void @llvm.dbg.value(metadata %struct.xdp_md* %0, metadata !471, metadata !DIExpression()), !dbg !472
  %2 = tail call i32 @xdpdecap(%struct.xdp_md* %0), !dbg !473
  ret i32 %2, !dbg !474
}

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #1

; Function Attrs: nounwind readnone speculatable willreturn
declare void @llvm.dbg.value(metadata, metadata, metadata) #2

attributes #0 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind willreturn }
attributes #2 = { nounwind readnone speculatable willreturn }
attributes #3 = { nounwind }

!llvm.dbg.cu = !{!2}
!llvm.module.flags = !{!129, !130, !131}
!llvm.ident = !{!132}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "_license", scope: !2, file: !3, line: 15, type: !125, isLocal: false, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C99, file: !3, producer: "clang version 10.0.0-4ubuntu1 ", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !4, retainedTypes: !43, globals: !50, splitDebugInlining: false, nameTableKind: None)
!3 = !DIFile(filename: "x.c", directory: "/home/sayandes/codequery/test")
!4 = !{!5, !14}
!5 = !DICompositeType(tag: DW_TAG_enumeration_type, name: "xdp_action", file: !6, line: 3112, baseType: !7, size: 32, elements: !8)
!6 = !DIFile(filename: "./bpf.h", directory: "/home/sayandes/codequery/test")
!7 = !DIBasicType(name: "unsigned int", size: 32, encoding: DW_ATE_unsigned)
!8 = !{!9, !10, !11, !12, !13}
!9 = !DIEnumerator(name: "XDP_ABORTED", value: 0, isUnsigned: true)
!10 = !DIEnumerator(name: "XDP_DROP", value: 1, isUnsigned: true)
!11 = !DIEnumerator(name: "XDP_PASS", value: 2, isUnsigned: true)
!12 = !DIEnumerator(name: "XDP_TX", value: 3, isUnsigned: true)
!13 = !DIEnumerator(name: "XDP_REDIRECT", value: 4, isUnsigned: true)
!14 = !DICompositeType(tag: DW_TAG_enumeration_type, file: !15, line: 28, baseType: !7, size: 32, elements: !16)
!15 = !DIFile(filename: "/usr/include/linux/in.h", directory: "")
!16 = !{!17, !18, !19, !20, !21, !22, !23, !24, !25, !26, !27, !28, !29, !30, !31, !32, !33, !34, !35, !36, !37, !38, !39, !40, !41, !42}
!17 = !DIEnumerator(name: "IPPROTO_IP", value: 0, isUnsigned: true)
!18 = !DIEnumerator(name: "IPPROTO_ICMP", value: 1, isUnsigned: true)
!19 = !DIEnumerator(name: "IPPROTO_IGMP", value: 2, isUnsigned: true)
!20 = !DIEnumerator(name: "IPPROTO_IPIP", value: 4, isUnsigned: true)
!21 = !DIEnumerator(name: "IPPROTO_TCP", value: 6, isUnsigned: true)
!22 = !DIEnumerator(name: "IPPROTO_EGP", value: 8, isUnsigned: true)
!23 = !DIEnumerator(name: "IPPROTO_PUP", value: 12, isUnsigned: true)
!24 = !DIEnumerator(name: "IPPROTO_UDP", value: 17, isUnsigned: true)
!25 = !DIEnumerator(name: "IPPROTO_IDP", value: 22, isUnsigned: true)
!26 = !DIEnumerator(name: "IPPROTO_TP", value: 29, isUnsigned: true)
!27 = !DIEnumerator(name: "IPPROTO_DCCP", value: 33, isUnsigned: true)
!28 = !DIEnumerator(name: "IPPROTO_IPV6", value: 41, isUnsigned: true)
!29 = !DIEnumerator(name: "IPPROTO_RSVP", value: 46, isUnsigned: true)
!30 = !DIEnumerator(name: "IPPROTO_GRE", value: 47, isUnsigned: true)
!31 = !DIEnumerator(name: "IPPROTO_ESP", value: 50, isUnsigned: true)
!32 = !DIEnumerator(name: "IPPROTO_AH", value: 51, isUnsigned: true)
!33 = !DIEnumerator(name: "IPPROTO_MTP", value: 92, isUnsigned: true)
!34 = !DIEnumerator(name: "IPPROTO_BEETPH", value: 94, isUnsigned: true)
!35 = !DIEnumerator(name: "IPPROTO_ENCAP", value: 98, isUnsigned: true)
!36 = !DIEnumerator(name: "IPPROTO_PIM", value: 103, isUnsigned: true)
!37 = !DIEnumerator(name: "IPPROTO_COMP", value: 108, isUnsigned: true)
!38 = !DIEnumerator(name: "IPPROTO_SCTP", value: 132, isUnsigned: true)
!39 = !DIEnumerator(name: "IPPROTO_UDPLITE", value: 136, isUnsigned: true)
!40 = !DIEnumerator(name: "IPPROTO_MPLS", value: 137, isUnsigned: true)
!41 = !DIEnumerator(name: "IPPROTO_RAW", value: 255, isUnsigned: true)
!42 = !DIEnumerator(name: "IPPROTO_MAX", value: 256, isUnsigned: true)
!43 = !{!44, !45, !46, !49}
!44 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 64)
!45 = !DIBasicType(name: "long int", size: 64, encoding: DW_ATE_signed)
!46 = !DIDerivedType(tag: DW_TAG_typedef, name: "__u16", file: !47, line: 24, baseType: !48)
!47 = !DIFile(filename: "/usr/include/asm-generic/int-ll64.h", directory: "")
!48 = !DIBasicType(name: "unsigned short", size: 16, encoding: DW_ATE_unsigned)
!49 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!50 = !{!0, !51, !83, !114, !120}
!51 = !DIGlobalVariableExpression(var: !52, expr: !DIExpression())
!52 = distinct !DIGlobalVariable(name: "decap_counters", scope: !2, file: !53, line: 44, type: !54, isLocal: false, isDefinition: true)
!53 = !DIFile(filename: "./decap_maps.h", directory: "/home/sayandes/codequery/test")
!54 = distinct !DICompositeType(tag: DW_TAG_structure_type, file: !53, line: 38, size: 320, elements: !55)
!55 = !{!56, !61, !64, !73, !78}
!56 = !DIDerivedType(tag: DW_TAG_member, name: "type", scope: !54, file: !53, line: 39, baseType: !57, size: 64)
!57 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !58, size: 64)
!58 = !DICompositeType(tag: DW_TAG_array_type, baseType: !49, size: 192, elements: !59)
!59 = !{!60}
!60 = !DISubrange(count: 6)
!61 = !DIDerivedType(tag: DW_TAG_member, name: "key", scope: !54, file: !53, line: 40, baseType: !62, size: 64, offset: 64)
!62 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !63, size: 64)
!63 = !DIDerivedType(tag: DW_TAG_typedef, name: "__u32", file: !47, line: 27, baseType: !7)
!64 = !DIDerivedType(tag: DW_TAG_member, name: "value", scope: !54, file: !53, line: 41, baseType: !65, size: 64, offset: 128)
!65 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !66, size: 64)
!66 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "decap_stats", file: !53, line: 31, size: 192, elements: !67)
!67 = !{!68, !71, !72}
!68 = !DIDerivedType(tag: DW_TAG_member, name: "decap_v4", scope: !66, file: !53, line: 32, baseType: !69, size: 64)
!69 = !DIDerivedType(tag: DW_TAG_typedef, name: "__u64", file: !47, line: 31, baseType: !70)
!70 = !DIBasicType(name: "long long unsigned int", size: 64, encoding: DW_ATE_unsigned)
!71 = !DIDerivedType(tag: DW_TAG_member, name: "decap_v6", scope: !66, file: !53, line: 33, baseType: !69, size: 64, offset: 64)
!72 = !DIDerivedType(tag: DW_TAG_member, name: "total", scope: !66, file: !53, line: 34, baseType: !69, size: 64, offset: 128)
!73 = !DIDerivedType(tag: DW_TAG_member, name: "max_entries", scope: !54, file: !53, line: 42, baseType: !74, size: 64, offset: 192)
!74 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !75, size: 64)
!75 = !DICompositeType(tag: DW_TAG_array_type, baseType: !49, size: 32768, elements: !76)
!76 = !{!77}
!77 = !DISubrange(count: 1024)
!78 = !DIDerivedType(tag: DW_TAG_member, name: "map_flags", scope: !54, file: !53, line: 43, baseType: !79, size: 64, offset: 256)
!79 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !80, size: 64)
!80 = !DICompositeType(tag: DW_TAG_array_type, baseType: !49, elements: !81)
!81 = !{!82}
!82 = !DISubrange(count: 0)
!83 = !DIGlobalVariableExpression(var: !84, expr: !DIExpression())
!84 = distinct !DIGlobalVariable(name: "ctl_array", scope: !2, file: !85, line: 41, type: !86, isLocal: false, isDefinition: true)
!85 = !DIFile(filename: "./control_data_maps.h", directory: "/home/sayandes/codequery/test")
!86 = distinct !DICompositeType(tag: DW_TAG_structure_type, file: !85, line: 35, size: 320, elements: !87)
!87 = !{!88, !93, !94, !108, !113}
!88 = !DIDerivedType(tag: DW_TAG_member, name: "type", scope: !86, file: !85, line: 36, baseType: !89, size: 64)
!89 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !90, size: 64)
!90 = !DICompositeType(tag: DW_TAG_array_type, baseType: !49, size: 64, elements: !91)
!91 = !{!92}
!92 = !DISubrange(count: 2)
!93 = !DIDerivedType(tag: DW_TAG_member, name: "key", scope: !86, file: !85, line: 37, baseType: !62, size: 64, offset: 64)
!94 = !DIDerivedType(tag: DW_TAG_member, name: "value", scope: !86, file: !85, line: 38, baseType: !95, size: 64, offset: 128)
!95 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !96, size: 64)
!96 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "ctl_value", file: !97, line: 53, size: 64, elements: !98)
!97 = !DIFile(filename: "./balancer_structs.h", directory: "/home/sayandes/codequery/test")
!98 = !{!99}
!99 = !DIDerivedType(tag: DW_TAG_member, scope: !96, file: !97, line: 54, baseType: !100, size: 64)
!100 = distinct !DICompositeType(tag: DW_TAG_union_type, scope: !96, file: !97, line: 54, size: 64, elements: !101)
!101 = !{!102, !103, !104}
!102 = !DIDerivedType(tag: DW_TAG_member, name: "value", scope: !100, file: !97, line: 55, baseType: !69, size: 64)
!103 = !DIDerivedType(tag: DW_TAG_member, name: "ifindex", scope: !100, file: !97, line: 56, baseType: !63, size: 32)
!104 = !DIDerivedType(tag: DW_TAG_member, name: "mac", scope: !100, file: !97, line: 57, baseType: !105, size: 48)
!105 = !DICompositeType(tag: DW_TAG_array_type, baseType: !106, size: 48, elements: !59)
!106 = !DIDerivedType(tag: DW_TAG_typedef, name: "__u8", file: !47, line: 21, baseType: !107)
!107 = !DIBasicType(name: "unsigned char", size: 8, encoding: DW_ATE_unsigned_char)
!108 = !DIDerivedType(tag: DW_TAG_member, name: "max_entries", scope: !86, file: !85, line: 39, baseType: !109, size: 64, offset: 192)
!109 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !110, size: 64)
!110 = !DICompositeType(tag: DW_TAG_array_type, baseType: !49, size: 512, elements: !111)
!111 = !{!112}
!112 = !DISubrange(count: 16)
!113 = !DIDerivedType(tag: DW_TAG_member, name: "map_flags", scope: !86, file: !85, line: 40, baseType: !79, size: 64, offset: 256)
!114 = !DIGlobalVariableExpression(var: !115, expr: !DIExpression())
!115 = distinct !DIGlobalVariable(name: "bpf_map_lookup_elem", scope: !2, file: !116, line: 22, type: !117, isLocal: true, isDefinition: true)
!116 = !DIFile(filename: "./bpf_helpers.h", directory: "/home/sayandes/codequery/test")
!117 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !118, size: 64)
!118 = !DISubroutineType(types: !119)
!119 = !{!44, !44, !44}
!120 = !DIGlobalVariableExpression(var: !121, expr: !DIExpression())
!121 = distinct !DIGlobalVariable(name: "bpf_xdp_adjust_head", scope: !2, file: !116, line: 91, type: !122, isLocal: true, isDefinition: true)
!122 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !123, size: 64)
!123 = !DISubroutineType(types: !124)
!124 = !{!49, !44, !49}
!125 = !DICompositeType(tag: DW_TAG_array_type, baseType: !126, size: 32, elements: !127)
!126 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!127 = !{!128}
!128 = !DISubrange(count: 4)
!129 = !{i32 7, !"Dwarf Version", i32 4}
!130 = !{i32 2, !"Debug Info Version", i32 3}
!131 = !{i32 1, !"wchar_size", i32 4}
!132 = !{!"clang version 10.0.0-4ubuntu1 "}
!133 = distinct !DISubprogram(name: "xdpdecap", scope: !134, file: !134, line: 224, type: !135, scopeLine: 224, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !145)
!134 = !DIFile(filename: "./decap_kern.c", directory: "/home/sayandes/codequery/test")
!135 = !DISubroutineType(types: !136)
!136 = !{!49, !137}
!137 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !138, size: 64)
!138 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "xdp_md", file: !6, line: 3123, size: 160, elements: !139)
!139 = !{!140, !141, !142, !143, !144}
!140 = !DIDerivedType(tag: DW_TAG_member, name: "data", scope: !138, file: !6, line: 3124, baseType: !63, size: 32)
!141 = !DIDerivedType(tag: DW_TAG_member, name: "data_end", scope: !138, file: !6, line: 3125, baseType: !63, size: 32, offset: 32)
!142 = !DIDerivedType(tag: DW_TAG_member, name: "data_meta", scope: !138, file: !6, line: 3126, baseType: !63, size: 32, offset: 64)
!143 = !DIDerivedType(tag: DW_TAG_member, name: "ingress_ifindex", scope: !138, file: !6, line: 3128, baseType: !63, size: 32, offset: 96)
!144 = !DIDerivedType(tag: DW_TAG_member, name: "rx_queue_index", scope: !138, file: !6, line: 3129, baseType: !63, size: 32, offset: 128)
!145 = !{!146, !147, !148, !149, !160, !161}
!146 = !DILocalVariable(name: "ctx", arg: 1, scope: !133, file: !134, line: 224, type: !137)
!147 = !DILocalVariable(name: "data", scope: !133, file: !134, line: 225, type: !44)
!148 = !DILocalVariable(name: "data_end", scope: !133, file: !134, line: 226, type: !44)
!149 = !DILocalVariable(name: "eth", scope: !133, file: !134, line: 227, type: !150)
!150 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !151, size: 64)
!151 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "ethhdr", file: !152, line: 163, size: 112, elements: !153)
!152 = !DIFile(filename: "/usr/include/linux/if_ether.h", directory: "")
!153 = !{!154, !156, !157}
!154 = !DIDerivedType(tag: DW_TAG_member, name: "h_dest", scope: !151, file: !152, line: 164, baseType: !155, size: 48)
!155 = !DICompositeType(tag: DW_TAG_array_type, baseType: !107, size: 48, elements: !59)
!156 = !DIDerivedType(tag: DW_TAG_member, name: "h_source", scope: !151, file: !152, line: 165, baseType: !155, size: 48, offset: 48)
!157 = !DIDerivedType(tag: DW_TAG_member, name: "h_proto", scope: !151, file: !152, line: 166, baseType: !158, size: 16, offset: 96)
!158 = !DIDerivedType(tag: DW_TAG_typedef, name: "__be16", file: !159, line: 25, baseType: !46)
!159 = !DIFile(filename: "/usr/include/linux/types.h", directory: "")
!160 = !DILocalVariable(name: "eth_proto", scope: !133, file: !134, line: 228, type: !63)
!161 = !DILocalVariable(name: "nh_off", scope: !133, file: !134, line: 229, type: !63)
!162 = !DILocation(line: 0, scope: !133)
!163 = !DILocation(line: 225, column: 34, scope: !133)
!164 = !{!165, !166, i64 0}
!165 = !{!"xdp_md", !166, i64 0, !166, i64 4, !166, i64 8, !166, i64 12, !166, i64 16}
!166 = !{!"int", !167, i64 0}
!167 = !{!"omnipotent char", !168, i64 0}
!168 = !{!"Simple C/C++ TBAA"}
!169 = !DILocation(line: 225, column: 23, scope: !133)
!170 = !DILocation(line: 225, column: 16, scope: !133)
!171 = !DILocation(line: 226, column: 38, scope: !133)
!172 = !{!165, !166, i64 4}
!173 = !DILocation(line: 226, column: 27, scope: !133)
!174 = !DILocation(line: 226, column: 20, scope: !133)
!175 = !DILocation(line: 232, column: 12, scope: !176)
!176 = distinct !DILexicalBlock(scope: !133, file: !134, line: 232, column: 7)
!177 = !DILocation(line: 232, column: 21, scope: !176)
!178 = !DILocation(line: 232, column: 7, scope: !133)
!179 = !DILocation(line: 227, column: 24, scope: !133)
!180 = !DILocation(line: 237, column: 20, scope: !133)
!181 = !{!182, !183, i64 12}
!182 = !{!"ethhdr", !167, i64 0, !167, i64 6, !183, i64 12}
!183 = !{!"short", !167, i64 0}
!184 = !DILocation(line: 239, column: 7, scope: !133)
!185 = !DILocalVariable(name: "data", arg: 1, scope: !186, file: !134, line: 165, type: !44)
!186 = distinct !DISubprogram(name: "process_packet", scope: !134, file: !134, line: 164, type: !187, scopeLine: 169, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !190)
!187 = !DISubroutineType(types: !188)
!188 = !{!49, !44, !69, !44, !189, !137}
!189 = !DIBasicType(name: "_Bool", size: 8, encoding: DW_ATE_boolean)
!190 = !{!185, !191, !192, !193, !194, !195, !223, !224, !225, !226, !227}
!191 = !DILocalVariable(name: "off", arg: 2, scope: !186, file: !134, line: 166, type: !69)
!192 = !DILocalVariable(name: "data_end", arg: 3, scope: !186, file: !134, line: 167, type: !44)
!193 = !DILocalVariable(name: "is_ipv6", arg: 4, scope: !186, file: !134, line: 168, type: !189)
!194 = !DILocalVariable(name: "xdp", arg: 5, scope: !186, file: !134, line: 169, type: !137)
!195 = !DILocalVariable(name: "pckt", scope: !186, file: !134, line: 170, type: !196)
!196 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "packet_description", file: !97, line: 43, size: 384, elements: !197)
!197 = !{!198, !220, !221, !222}
!198 = !DIDerivedType(tag: DW_TAG_member, name: "flow", scope: !196, file: !97, line: 44, baseType: !199, size: 320)
!199 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "flow_key", file: !97, line: 26, size: 320, elements: !200)
!200 = !{!201, !208, !213, !219}
!201 = !DIDerivedType(tag: DW_TAG_member, scope: !199, file: !97, line: 27, baseType: !202, size: 128)
!202 = distinct !DICompositeType(tag: DW_TAG_union_type, scope: !199, file: !97, line: 27, size: 128, elements: !203)
!203 = !{!204, !206}
!204 = !DIDerivedType(tag: DW_TAG_member, name: "src", scope: !202, file: !97, line: 28, baseType: !205, size: 32)
!205 = !DIDerivedType(tag: DW_TAG_typedef, name: "__be32", file: !159, line: 27, baseType: !63)
!206 = !DIDerivedType(tag: DW_TAG_member, name: "srcv6", scope: !202, file: !97, line: 29, baseType: !207, size: 128)
!207 = !DICompositeType(tag: DW_TAG_array_type, baseType: !205, size: 128, elements: !127)
!208 = !DIDerivedType(tag: DW_TAG_member, scope: !199, file: !97, line: 31, baseType: !209, size: 128, offset: 128)
!209 = distinct !DICompositeType(tag: DW_TAG_union_type, scope: !199, file: !97, line: 31, size: 128, elements: !210)
!210 = !{!211, !212}
!211 = !DIDerivedType(tag: DW_TAG_member, name: "dst", scope: !209, file: !97, line: 32, baseType: !205, size: 32)
!212 = !DIDerivedType(tag: DW_TAG_member, name: "dstv6", scope: !209, file: !97, line: 33, baseType: !207, size: 128)
!213 = !DIDerivedType(tag: DW_TAG_member, scope: !199, file: !97, line: 35, baseType: !214, size: 32, offset: 256)
!214 = distinct !DICompositeType(tag: DW_TAG_union_type, scope: !199, file: !97, line: 35, size: 32, elements: !215)
!215 = !{!216, !217}
!216 = !DIDerivedType(tag: DW_TAG_member, name: "ports", scope: !214, file: !97, line: 36, baseType: !63, size: 32)
!217 = !DIDerivedType(tag: DW_TAG_member, name: "port16", scope: !214, file: !97, line: 37, baseType: !218, size: 32)
!218 = !DICompositeType(tag: DW_TAG_array_type, baseType: !46, size: 32, elements: !91)
!219 = !DIDerivedType(tag: DW_TAG_member, name: "proto", scope: !199, file: !97, line: 39, baseType: !106, size: 8, offset: 288)
!220 = !DIDerivedType(tag: DW_TAG_member, name: "real_index", scope: !196, file: !97, line: 45, baseType: !63, size: 32, offset: 320)
!221 = !DIDerivedType(tag: DW_TAG_member, name: "flags", scope: !196, file: !97, line: 46, baseType: !106, size: 8, offset: 352)
!222 = !DIDerivedType(tag: DW_TAG_member, name: "tos", scope: !196, file: !97, line: 48, baseType: !106, size: 8, offset: 360)
!223 = !DILocalVariable(name: "data_stats", scope: !186, file: !134, line: 171, type: !65)
!224 = !DILocalVariable(name: "key", scope: !186, file: !134, line: 172, type: !63)
!225 = !DILocalVariable(name: "protocol", scope: !186, file: !134, line: 173, type: !106)
!226 = !DILocalVariable(name: "action", scope: !186, file: !134, line: 175, type: !49)
!227 = !DILocalVariable(name: "pkt_bytes", scope: !186, file: !134, line: 176, type: !46)
!228 = !DILocation(line: 0, scope: !186, inlinedAt: !229)
!229 = distinct !DILocation(line: 240, column: 12, scope: !230)
!230 = distinct !DILexicalBlock(scope: !231, file: !134, line: 239, column: 33)
!231 = distinct !DILexicalBlock(scope: !133, file: !134, line: 239, column: 7)
!232 = !DILocation(line: 172, column: 3, scope: !186, inlinedAt: !229)
!233 = !DILocation(line: 172, column: 9, scope: !186, inlinedAt: !229)
!234 = !{!166, !166, i64 0}
!235 = !DILocalVariable(name: "pckt", arg: 1, scope: !236, file: !134, line: 35, type: !239)
!236 = distinct !DISubprogram(name: "process_l3_headers", scope: !134, file: !134, line: 34, type: !237, scopeLine: 41, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !242)
!237 = !DISubroutineType(types: !238)
!238 = !{!49, !239, !240, !69, !241, !44, !44, !189}
!239 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !196, size: 64)
!240 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !106, size: 64)
!241 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !46, size: 64)
!242 = !{!235, !243, !244, !245, !246, !247, !248, !249, !250, !267}
!243 = !DILocalVariable(name: "protocol", arg: 2, scope: !236, file: !134, line: 36, type: !240)
!244 = !DILocalVariable(name: "off", arg: 3, scope: !236, file: !134, line: 37, type: !69)
!245 = !DILocalVariable(name: "pkt_bytes", arg: 4, scope: !236, file: !134, line: 38, type: !241)
!246 = !DILocalVariable(name: "data", arg: 5, scope: !236, file: !134, line: 39, type: !44)
!247 = !DILocalVariable(name: "data_end", arg: 6, scope: !236, file: !134, line: 40, type: !44)
!248 = !DILocalVariable(name: "is_ipv6", arg: 7, scope: !236, file: !134, line: 41, type: !189)
!249 = !DILocalVariable(name: "iph_len", scope: !236, file: !134, line: 42, type: !69)
!250 = !DILocalVariable(name: "iph", scope: !236, file: !134, line: 43, type: !251)
!251 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !252, size: 64)
!252 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "iphdr", file: !253, line: 86, size: 160, elements: !254)
!253 = !DIFile(filename: "/usr/include/linux/ip.h", directory: "")
!254 = !{!255, !256, !257, !258, !259, !260, !261, !262, !263, !265, !266}
!255 = !DIDerivedType(tag: DW_TAG_member, name: "ihl", scope: !252, file: !253, line: 88, baseType: !106, size: 4, flags: DIFlagBitField, extraData: i64 0)
!256 = !DIDerivedType(tag: DW_TAG_member, name: "version", scope: !252, file: !253, line: 89, baseType: !106, size: 4, offset: 4, flags: DIFlagBitField, extraData: i64 0)
!257 = !DIDerivedType(tag: DW_TAG_member, name: "tos", scope: !252, file: !253, line: 96, baseType: !106, size: 8, offset: 8)
!258 = !DIDerivedType(tag: DW_TAG_member, name: "tot_len", scope: !252, file: !253, line: 97, baseType: !158, size: 16, offset: 16)
!259 = !DIDerivedType(tag: DW_TAG_member, name: "id", scope: !252, file: !253, line: 98, baseType: !158, size: 16, offset: 32)
!260 = !DIDerivedType(tag: DW_TAG_member, name: "frag_off", scope: !252, file: !253, line: 99, baseType: !158, size: 16, offset: 48)
!261 = !DIDerivedType(tag: DW_TAG_member, name: "ttl", scope: !252, file: !253, line: 100, baseType: !106, size: 8, offset: 64)
!262 = !DIDerivedType(tag: DW_TAG_member, name: "protocol", scope: !252, file: !253, line: 101, baseType: !106, size: 8, offset: 72)
!263 = !DIDerivedType(tag: DW_TAG_member, name: "check", scope: !252, file: !253, line: 102, baseType: !264, size: 16, offset: 80)
!264 = !DIDerivedType(tag: DW_TAG_typedef, name: "__sum16", file: !159, line: 31, baseType: !46)
!265 = !DIDerivedType(tag: DW_TAG_member, name: "saddr", scope: !252, file: !253, line: 103, baseType: !205, size: 32, offset: 96)
!266 = !DIDerivedType(tag: DW_TAG_member, name: "daddr", scope: !252, file: !253, line: 104, baseType: !205, size: 32, offset: 128)
!267 = !DILocalVariable(name: "ip6h", scope: !236, file: !134, line: 44, type: !268)
!268 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !269, size: 64)
!269 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "ipv6hdr", file: !270, line: 116, size: 320, elements: !271)
!270 = !DIFile(filename: "/usr/include/linux/ipv6.h", directory: "")
!271 = !{!272, !273, !274, !278, !279, !280, !281, !295}
!272 = !DIDerivedType(tag: DW_TAG_member, name: "priority", scope: !269, file: !270, line: 118, baseType: !106, size: 4, flags: DIFlagBitField, extraData: i64 0)
!273 = !DIDerivedType(tag: DW_TAG_member, name: "version", scope: !269, file: !270, line: 119, baseType: !106, size: 4, offset: 4, flags: DIFlagBitField, extraData: i64 0)
!274 = !DIDerivedType(tag: DW_TAG_member, name: "flow_lbl", scope: !269, file: !270, line: 126, baseType: !275, size: 24, offset: 8)
!275 = !DICompositeType(tag: DW_TAG_array_type, baseType: !106, size: 24, elements: !276)
!276 = !{!277}
!277 = !DISubrange(count: 3)
!278 = !DIDerivedType(tag: DW_TAG_member, name: "payload_len", scope: !269, file: !270, line: 128, baseType: !158, size: 16, offset: 32)
!279 = !DIDerivedType(tag: DW_TAG_member, name: "nexthdr", scope: !269, file: !270, line: 129, baseType: !106, size: 8, offset: 48)
!280 = !DIDerivedType(tag: DW_TAG_member, name: "hop_limit", scope: !269, file: !270, line: 130, baseType: !106, size: 8, offset: 56)
!281 = !DIDerivedType(tag: DW_TAG_member, name: "saddr", scope: !269, file: !270, line: 132, baseType: !282, size: 128, offset: 64)
!282 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "in6_addr", file: !283, line: 33, size: 128, elements: !284)
!283 = !DIFile(filename: "/usr/include/linux/in6.h", directory: "")
!284 = !{!285}
!285 = !DIDerivedType(tag: DW_TAG_member, name: "in6_u", scope: !282, file: !283, line: 40, baseType: !286, size: 128)
!286 = distinct !DICompositeType(tag: DW_TAG_union_type, scope: !282, file: !283, line: 34, size: 128, elements: !287)
!287 = !{!288, !290, !294}
!288 = !DIDerivedType(tag: DW_TAG_member, name: "u6_addr8", scope: !286, file: !283, line: 35, baseType: !289, size: 128)
!289 = !DICompositeType(tag: DW_TAG_array_type, baseType: !106, size: 128, elements: !111)
!290 = !DIDerivedType(tag: DW_TAG_member, name: "u6_addr16", scope: !286, file: !283, line: 37, baseType: !291, size: 128)
!291 = !DICompositeType(tag: DW_TAG_array_type, baseType: !158, size: 128, elements: !292)
!292 = !{!293}
!293 = !DISubrange(count: 8)
!294 = !DIDerivedType(tag: DW_TAG_member, name: "u6_addr32", scope: !286, file: !283, line: 38, baseType: !207, size: 128)
!295 = !DIDerivedType(tag: DW_TAG_member, name: "daddr", scope: !269, file: !270, line: 133, baseType: !282, size: 128, offset: 192)
!296 = !DILocation(line: 0, scope: !236, inlinedAt: !297)
!297 = distinct !DILocation(line: 177, column: 12, scope: !186, inlinedAt: !229)
!298 = !DILocation(line: 62, column: 13, scope: !299, inlinedAt: !297)
!299 = distinct !DILexicalBlock(scope: !300, file: !134, line: 62, column: 9)
!300 = distinct !DILexicalBlock(scope: !301, file: !134, line: 60, column: 10)
!301 = distinct !DILexicalBlock(scope: !236, file: !134, line: 45, column: 7)
!302 = !DILocation(line: 62, column: 17, scope: !299, inlinedAt: !297)
!303 = !DILocation(line: 62, column: 9, scope: !300, inlinedAt: !297)
!304 = !DILocation(line: 66, column: 14, scope: !305, inlinedAt: !297)
!305 = distinct !DILexicalBlock(scope: !300, file: !134, line: 66, column: 9)
!306 = !DILocation(line: 66, column: 18, scope: !305, inlinedAt: !297)
!307 = !DILocation(line: 66, column: 9, scope: !300, inlinedAt: !297)
!308 = !DILocation(line: 72, column: 22, scope: !300, inlinedAt: !297)
!309 = !{!310, !167, i64 9}
!310 = !{!"iphdr", !167, i64 0, !167, i64 0, !167, i64 1, !183, i64 2, !183, i64 4, !183, i64 6, !167, i64 8, !167, i64 9, !183, i64 10, !166, i64 12, !166, i64 16}
!311 = !DILocation(line: 77, column: 14, scope: !312, inlinedAt: !297)
!312 = distinct !DILexicalBlock(scope: !300, file: !134, line: 77, column: 9)
!313 = !{!310, !183, i64 6}
!314 = !DILocation(line: 77, column: 23, scope: !312, inlinedAt: !297)
!315 = !DILocation(line: 77, column: 9, scope: !300, inlinedAt: !297)
!316 = !DILocation(line: 184, column: 16, scope: !186, inlinedAt: !229)
!317 = !DILocation(line: 185, column: 8, scope: !318, inlinedAt: !229)
!318 = distinct !DILexicalBlock(scope: !186, file: !134, line: 185, column: 7)
!319 = !DILocation(line: 185, column: 7, scope: !186, inlinedAt: !229)
!320 = !DILocation(line: 189, column: 15, scope: !186, inlinedAt: !229)
!321 = !DILocation(line: 189, column: 21, scope: !186, inlinedAt: !229)
!322 = !{!323, !324, i64 16}
!323 = !{!"decap_stats", !324, i64 0, !324, i64 8, !324, i64 16}
!324 = !{!"long long", !167, i64 0}
!325 = !DILocation(line: 190, column: 32, scope: !326, inlinedAt: !229)
!326 = distinct !DILexicalBlock(scope: !186, file: !134, line: 190, column: 7)
!327 = !DILocation(line: 194, column: 19, scope: !328, inlinedAt: !229)
!328 = distinct !DILexicalBlock(scope: !329, file: !134, line: 193, column: 12)
!329 = distinct !DILexicalBlock(scope: !330, file: !134, line: 191, column: 9)
!330 = distinct !DILexicalBlock(scope: !326, file: !134, line: 190, column: 61)
!331 = !DILocation(line: 194, column: 28, scope: !328, inlinedAt: !229)
!332 = !{!323, !324, i64 0}
!333 = !DILocalVariable(name: "xdp", arg: 3, scope: !334, file: !134, line: 88, type: !137)
!334 = distinct !DISubprogram(name: "process_encaped_ipip_pckt", scope: !134, file: !134, line: 85, type: !335, scopeLine: 93, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !339)
!335 = !DISubroutineType(types: !336)
!336 = !{!49, !337, !337, !137, !338, !239, !240, !69, !241}
!337 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !44, size: 64)
!338 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !189, size: 64)
!339 = !{!340, !341, !333, !342, !343, !344, !345, !346}
!340 = !DILocalVariable(name: "data", arg: 1, scope: !334, file: !134, line: 86, type: !337)
!341 = !DILocalVariable(name: "data_end", arg: 2, scope: !334, file: !134, line: 87, type: !337)
!342 = !DILocalVariable(name: "is_ipv6", arg: 4, scope: !334, file: !134, line: 89, type: !338)
!343 = !DILocalVariable(name: "pckt", arg: 5, scope: !334, file: !134, line: 90, type: !239)
!344 = !DILocalVariable(name: "protocol", arg: 6, scope: !334, file: !134, line: 91, type: !240)
!345 = !DILocalVariable(name: "off", arg: 7, scope: !334, file: !134, line: 92, type: !69)
!346 = !DILocalVariable(name: "pkt_bytes", arg: 8, scope: !334, file: !134, line: 93, type: !241)
!347 = !DILocation(line: 0, scope: !334, inlinedAt: !348)
!348 = distinct !DILocation(line: 196, column: 14, scope: !330, inlinedAt: !229)
!349 = !DILocation(line: 94, column: 7, scope: !334, inlinedAt: !348)
!350 = !DILocalVariable(name: "xdp", arg: 1, scope: !351, file: !352, line: 159, type: !137)
!351 = distinct !DISubprogram(name: "decap_v4", scope: !352, file: !352, line: 159, type: !353, scopeLine: 159, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !355)
!352 = !DIFile(filename: "./pckt_encap.h", directory: "/home/sayandes/codequery/test")
!353 = !DISubroutineType(types: !354)
!354 = !{!189, !137, !337, !337}
!355 = !{!350, !356, !357, !358, !359}
!356 = !DILocalVariable(name: "data", arg: 2, scope: !351, file: !352, line: 159, type: !337)
!357 = !DILocalVariable(name: "data_end", arg: 3, scope: !351, file: !352, line: 159, type: !337)
!358 = !DILocalVariable(name: "new_eth", scope: !351, file: !352, line: 160, type: !150)
!359 = !DILocalVariable(name: "old_eth", scope: !351, file: !352, line: 161, type: !150)
!360 = !DILocation(line: 0, scope: !351, inlinedAt: !361)
!361 = distinct !DILocation(line: 107, column: 12, scope: !362, inlinedAt: !348)
!362 = distinct !DILexicalBlock(scope: !363, file: !134, line: 107, column: 11)
!363 = distinct !DILexicalBlock(scope: !364, file: !134, line: 103, column: 12)
!364 = distinct !DILexicalBlock(scope: !365, file: !134, line: 95, column: 9)
!365 = distinct !DILexicalBlock(scope: !366, file: !134, line: 94, column: 34)
!366 = distinct !DILexicalBlock(scope: !334, file: !134, line: 94, column: 7)
!367 = !DILocation(line: 164, column: 19, scope: !351, inlinedAt: !361)
!368 = !DILocation(line: 164, column: 38, scope: !351, inlinedAt: !361)
!369 = !DILocation(line: 164, column: 3, scope: !351, inlinedAt: !361)
!370 = !DILocation(line: 165, column: 3, scope: !351, inlinedAt: !361)
!371 = !DILocation(line: 166, column: 12, scope: !351, inlinedAt: !361)
!372 = !DILocation(line: 166, column: 20, scope: !351, inlinedAt: !361)
!373 = !DILocation(line: 167, column: 27, scope: !374, inlinedAt: !361)
!374 = distinct !DILexicalBlock(scope: !351, file: !352, line: 167, column: 7)
!375 = !DILocation(line: 167, column: 7, scope: !374, inlinedAt: !361)
!376 = !DILocation(line: 167, column: 7, scope: !351, inlinedAt: !361)
!377 = !DILocation(line: 112, column: 41, scope: !378, inlinedAt: !348)
!378 = distinct !DILexicalBlock(scope: !379, file: !134, line: 112, column: 9)
!379 = distinct !DILexicalBlock(scope: !380, file: !134, line: 111, column: 41)
!380 = distinct !DILexicalBlock(scope: !366, file: !134, line: 111, column: 14)
!381 = !DILocation(line: 112, column: 66, scope: !378, inlinedAt: !348)
!382 = !DILocation(line: 112, column: 9, scope: !379, inlinedAt: !348)
!383 = !DILocalVariable(name: "xdp", arg: 1, scope: !384, file: !352, line: 138, type: !137)
!384 = distinct !DISubprogram(name: "decap_v6", scope: !352, file: !352, line: 138, type: !385, scopeLine: 138, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !387)
!385 = !DISubroutineType(types: !386)
!386 = !{!189, !137, !337, !337, !189}
!387 = !{!383, !388, !389, !390, !391, !392}
!388 = !DILocalVariable(name: "data", arg: 2, scope: !384, file: !352, line: 138, type: !337)
!389 = !DILocalVariable(name: "data_end", arg: 3, scope: !384, file: !352, line: 138, type: !337)
!390 = !DILocalVariable(name: "inner_v4", arg: 4, scope: !384, file: !352, line: 138, type: !189)
!391 = !DILocalVariable(name: "new_eth", scope: !384, file: !352, line: 139, type: !150)
!392 = !DILocalVariable(name: "old_eth", scope: !384, file: !352, line: 140, type: !150)
!393 = !DILocation(line: 0, scope: !384, inlinedAt: !394)
!394 = distinct !DILocation(line: 115, column: 10, scope: !395, inlinedAt: !348)
!395 = distinct !DILexicalBlock(scope: !379, file: !134, line: 115, column: 9)
!396 = !DILocation(line: 142, column: 19, scope: !384, inlinedAt: !394)
!397 = !DILocation(line: 143, column: 19, scope: !384, inlinedAt: !394)
!398 = !DILocation(line: 143, column: 38, scope: !384, inlinedAt: !394)
!399 = !DILocation(line: 143, column: 3, scope: !384, inlinedAt: !394)
!400 = !DILocation(line: 144, column: 3, scope: !384, inlinedAt: !394)
!401 = !DILocation(line: 0, scope: !402, inlinedAt: !394)
!402 = distinct !DILexicalBlock(scope: !384, file: !352, line: 145, column: 7)
!403 = !DILocation(line: 150, column: 27, scope: !404, inlinedAt: !394)
!404 = distinct !DILexicalBlock(scope: !384, file: !352, line: 150, column: 7)
!405 = !DILocation(line: 150, column: 7, scope: !404, inlinedAt: !394)
!406 = !DILocation(line: 150, column: 7, scope: !384, inlinedAt: !394)
!407 = !DILocation(line: 221, column: 1, scope: !186, inlinedAt: !229)
!408 = !DILocation(line: 240, column: 5, scope: !230)
!409 = !DILocation(line: 0, scope: !186, inlinedAt: !410)
!410 = distinct !DILocation(line: 242, column: 12, scope: !411)
!411 = distinct !DILexicalBlock(scope: !412, file: !134, line: 241, column: 42)
!412 = distinct !DILexicalBlock(scope: !231, file: !134, line: 241, column: 14)
!413 = !DILocation(line: 172, column: 3, scope: !186, inlinedAt: !410)
!414 = !DILocation(line: 172, column: 9, scope: !186, inlinedAt: !410)
!415 = !DILocation(line: 0, scope: !236, inlinedAt: !416)
!416 = distinct !DILocation(line: 177, column: 12, scope: !186, inlinedAt: !410)
!417 = !DILocation(line: 47, column: 14, scope: !418, inlinedAt: !416)
!418 = distinct !DILexicalBlock(scope: !419, file: !134, line: 47, column: 9)
!419 = distinct !DILexicalBlock(scope: !301, file: !134, line: 45, column: 16)
!420 = !DILocation(line: 47, column: 18, scope: !418, inlinedAt: !416)
!421 = !DILocation(line: 47, column: 9, scope: !419, inlinedAt: !416)
!422 = !DILocation(line: 52, column: 23, scope: !419, inlinedAt: !416)
!423 = !{!424, !167, i64 6}
!424 = !{!"ipv6hdr", !167, i64 0, !167, i64 0, !167, i64 1, !183, i64 4, !167, i64 6, !167, i64 7, !425, i64 8, !425, i64 24}
!425 = !{!"in6_addr", !167, i64 0}
!426 = !DILocation(line: 56, column: 19, scope: !427, inlinedAt: !416)
!427 = distinct !DILexicalBlock(scope: !419, file: !134, line: 56, column: 9)
!428 = !DILocation(line: 56, column: 9, scope: !419, inlinedAt: !416)
!429 = !DILocation(line: 184, column: 16, scope: !186, inlinedAt: !410)
!430 = !DILocation(line: 185, column: 8, scope: !318, inlinedAt: !410)
!431 = !DILocation(line: 185, column: 7, scope: !186, inlinedAt: !410)
!432 = !DILocation(line: 189, column: 15, scope: !186, inlinedAt: !410)
!433 = !DILocation(line: 189, column: 21, scope: !186, inlinedAt: !410)
!434 = !DILocation(line: 190, column: 32, scope: !326, inlinedAt: !410)
!435 = !DILocation(line: 192, column: 19, scope: !436, inlinedAt: !410)
!436 = distinct !DILexicalBlock(scope: !329, file: !134, line: 191, column: 18)
!437 = !DILocation(line: 192, column: 28, scope: !436, inlinedAt: !410)
!438 = !{!323, !324, i64 8}
!439 = !DILocation(line: 0, scope: !334, inlinedAt: !440)
!440 = distinct !DILocation(line: 196, column: 14, scope: !330, inlinedAt: !410)
!441 = !DILocation(line: 94, column: 7, scope: !334, inlinedAt: !440)
!442 = !DILocation(line: 0, scope: !384, inlinedAt: !443)
!443 = distinct !DILocation(line: 100, column: 12, scope: !444, inlinedAt: !440)
!444 = distinct !DILexicalBlock(scope: !445, file: !134, line: 100, column: 11)
!445 = distinct !DILexicalBlock(scope: !364, file: !134, line: 95, column: 19)
!446 = !DILocation(line: 142, column: 19, scope: !384, inlinedAt: !443)
!447 = !DILocation(line: 143, column: 19, scope: !384, inlinedAt: !443)
!448 = !DILocation(line: 143, column: 38, scope: !384, inlinedAt: !443)
!449 = !DILocation(line: 143, column: 3, scope: !384, inlinedAt: !443)
!450 = !DILocation(line: 144, column: 3, scope: !384, inlinedAt: !443)
!451 = !DILocation(line: 0, scope: !402, inlinedAt: !443)
!452 = !DILocation(line: 150, column: 27, scope: !404, inlinedAt: !443)
!453 = !DILocation(line: 150, column: 7, scope: !404, inlinedAt: !443)
!454 = !DILocation(line: 150, column: 7, scope: !384, inlinedAt: !443)
!455 = !DILocation(line: 0, scope: !384, inlinedAt: !456)
!456 = distinct !DILocation(line: 115, column: 10, scope: !395, inlinedAt: !440)
!457 = !DILocation(line: 142, column: 19, scope: !384, inlinedAt: !456)
!458 = !DILocation(line: 143, column: 19, scope: !384, inlinedAt: !456)
!459 = !DILocation(line: 143, column: 38, scope: !384, inlinedAt: !456)
!460 = !DILocation(line: 143, column: 3, scope: !384, inlinedAt: !456)
!461 = !DILocation(line: 144, column: 3, scope: !384, inlinedAt: !456)
!462 = !DILocation(line: 0, scope: !402, inlinedAt: !456)
!463 = !DILocation(line: 150, column: 27, scope: !404, inlinedAt: !456)
!464 = !DILocation(line: 150, column: 7, scope: !404, inlinedAt: !456)
!465 = !DILocation(line: 150, column: 7, scope: !384, inlinedAt: !456)
!466 = !DILocation(line: 221, column: 1, scope: !186, inlinedAt: !410)
!467 = !DILocation(line: 242, column: 5, scope: !411)
!468 = !DILocation(line: 247, column: 1, scope: !133)
!469 = distinct !DISubprogram(name: "xdp_prog_simple", scope: !3, file: !3, line: 9, type: !135, scopeLine: 10, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !470)
!470 = !{!471}
!471 = !DILocalVariable(name: "ctx", arg: 1, scope: !469, file: !3, line: 9, type: !137)
!472 = !DILocation(line: 0, scope: !469)
!473 = !DILocation(line: 11, column: 10, scope: !469)
!474 = !DILocation(line: 11, column: 3, scope: !469)
