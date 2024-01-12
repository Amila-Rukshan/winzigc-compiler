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
  store i32 32, i32* @i, align 4
  store i32 5, i32* @j, align 4
  %0 = load i32, i32* @i, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @0, i32 0, i32 0), i32 %0)
  %2 = load i32, i32* @j, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @1, i32 0, i32 0), i32 %2)
  ret i32 0
}
