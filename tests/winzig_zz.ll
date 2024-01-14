; ModuleID = 'MyProg'
source_filename = "MyProg"

@i = internal global i32 0
@j = internal global i32 0
@b = internal global i1 false
@k = internal global i1 false
@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %i = load i32, i32* @i, align 4
  %multmp = mul i32 3, %i
  %subtmp = sub i32 15, %multmp
  store i32 %subtmp, i32* @i, align 4
  %i1 = load i32, i32* @i, align 4
  %addtmp = add i32 %i1, 1
  store i32 %addtmp, i32* @j, align 4
  %i2 = load i32, i32* @i, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), i32 %i2)
  %j = load i32, i32* @j, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @1, i32 0, i32 0), i32 %j)
  ret i32 0
}
