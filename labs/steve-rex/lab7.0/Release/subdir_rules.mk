################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Timer.obj: ../Timer.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --gcc --define=ccs="ccs" --define=PART_TM4C1231E6PM --diag_wrap=off --diag_warning=225 --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="Timer.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

button.obj: ../button.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --gcc --define=ccs="ccs" --define=PART_TM4C1231E6PM --diag_wrap=off --diag_warning=225 --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="button.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

lcd.obj: ../lcd.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --gcc --define=ccs="ccs" --define=PART_TM4C1231E6PM --diag_wrap=off --diag_warning=225 --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="lcd.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

tm4c1231e6pm_startup_ccs.obj: ../tm4c1231e6pm_startup_ccs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --gcc --define=ccs="ccs" --define=PART_TM4C1231E6PM --diag_wrap=off --diag_warning=225 --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="tm4c1231e6pm_startup_ccs.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


