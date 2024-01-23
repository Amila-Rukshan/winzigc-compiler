; ModuleID = 'MyProg'
source_filename = "MyProg"

@i = internal global i32 0
@j = internal global i32 0
@b = internal global i1 false
@k = internal global i1 false
@0 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@1 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@3 = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1

declare i32 @printf(i8*, ...)

declare i32 @scanf(i8*, ...)

define i32 @main() {
entry:
  %0 = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @0, i32 0, i32 0), i32* @i)
  %1 = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @1, i32 0, i32 0), i32* @j)
  %i = load i32, i32* @i, align 4
  %j = load i32, i32* @j, align 4
  %modtmp = srem i32 %i, %j
  %netmp = icmp ne i32 %modtmp, 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @2, i32 0, i32 0), i1 %netmp)
  %i1 = load i32, i32* @i, align 4
  %subtmp = sub i32 100, %i1
  store i32 %subtmp, i32* @i, align 4
  %i2 = load i32, i32* @i, align 4
  %multmp = mul i32 2, %i2
  %j3 = load i32, i32* @j, align 4
  %addtmp = add i32 %multmp, %j3
  store i32 %addtmp, i32* @j, align 4
  %i4 = load i32, i32* @i, align 4
  %j5 = load i32, i32* @j, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @3, i32 0, i32 0), i32 %i4, i32 %j5)
  ret i32 0
}
