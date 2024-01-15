; ModuleID = 'MyProg'
source_filename = "MyProg"

@i = internal global i32 0
@j = internal global i32 0
@b = internal global i1 false
@k = internal global i1 false
@0 = private unnamed_addr constant [10 x i8] c"%d %d %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %i = load i32, i32* @i, align 4
  %multmp = mul i32 3, %i
  %subtmp = sub i32 15, %multmp
  store i32 %subtmp, i32* @i, align 4
  %i1 = load i32, i32* @i, align 4
  %addtmp = add i32 %i1, 7
  store i32 %addtmp, i32* @j, align 4
  %i2 = load i32, i32* @i, align 4
  %multmp3 = mul i32 2, %i2
  %j = load i32, i32* @j, align 4
  %addtmp4 = add i32 %multmp3, %j
  store i32 %addtmp4, i32* @j, align 4
  %i5 = load i32, i32* @i, align 4
  %j6 = load i32, i32* @j, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @0, i32 0, i32 0), i32 %i5, i32 %j6, i32 2)
  ret i32 0
}
