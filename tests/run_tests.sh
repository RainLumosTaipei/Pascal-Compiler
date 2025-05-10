#!/bin/bash

# 配置项
PASCC="./PASCC"           # Pascal转C工具路径
TEST_DIR="test_cases"     # 测试用例目录
OUTPUT_DIR="output"       # 临时输出目录
LOG_FILE="test_report.log" # 测试日志

# 清理旧文件
rm -rf $OUTPUT_DIR $LOG_FILE
mkdir -p $OUTPUT_DIR

# 遍历所有用例
for CASE_PATH in $TEST_DIR/*; do
  CASE_NAME=$(basename $CASE_PATH)
  INPUT_PAS="$CASE_PATH/input.pas"
  EXPECTED_OUT="$CASE_PATH/expected.txt"
  OUTPUT_C="$OUTPUT_DIR/$CASE_NAME.c"
  OUTPUT_EXE="$OUTPUT_DIR/$CASE_NAME"
  ACTUAL_OUT="$OUTPUT_DIR/$CASE_NAME.actual.txt"

  echo "=== Testing $CASE_NAME ===" >> $LOG_FILE

  # 1. Pascal转C
  $PASCC -i $INPUT_PAS -o $OUTPUT_C 2>> $LOG_FILE
  if [ $? -ne 0 ]; then
    echo "❌ $CASE_NAME: Pascal to C failed" >> $LOG_FILE
    continue
  fi

  # 2. 编译C代码
  gcc $OUTPUT_C -o $OUTPUT_EXE -lm 2>> $LOG_FILE
  if [ $? -ne 0 ]; then
    echo "❌ $CASE_NAME: C compilation failed" >> $LOG_FILE
    continue
  fi

  # 3. 运行程序
  $OUTPUT_EXE > $ACTUAL_OUT 2>&1

  # 4. 对比输出
  diff -w -B $EXPECTED_OUT $ACTUAL_OUT >> $LOG_FILE
  if [ $? -eq 0 ]; then
    echo "✅ $CASE_NAME: Passed" >> $LOG_FILE
  else
    echo "❌ $CASE_NAME: Output mismatch" >> $LOG_FILE
    echo "--- Expected ---" >> $LOG_FILE
    cat $EXPECTED_OUT >> $LOG_FILE
    echo "--- Actual ---" >> $LOG_FILE
    cat $ACTUAL_OUT >> $LOG_FILE
  fi
done

# 打印总结
echo "=== Test Summary ===" >> $LOG_FILE
grep "✅" $LOG_FILE | wc -l | xargs echo "Passed: " >> $LOG_FILE
grep "❌" $LOG_FILE | wc -l | xargs echo "Failed: " >> $LOG_FILE

# 显示日志
cat $LOG_FILE