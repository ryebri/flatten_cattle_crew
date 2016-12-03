################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
button.obj: ../button.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/TivaWare_C_Series-2.1.2.111" --include_path="/include" -g --gcc --define=ccs="ccs" --define=PART_TM4C1231E6PM --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="button.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

clock_intr.obj: ../clock_intr.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/TivaWare_C_Series-2.1.2.111" --include_path="/include" -g --gcc --define=ccs="ccs" --define=PART_TM4C1231E6PM --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="clock_intr.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

lcd.obj: ../lcd.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/TivaWare_C_Series-2.1.2.111" --include_path="/include" -g --gcc --define=ccs="ccs" --define=PART_TM4C1231E6PM --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="lcd.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

timer.obj: ../timer.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/TivaWare_C_Series-2.1.2.111" --include_path="/include" -g --gcc --define=ccs="ccs" --define=PART_TM4C1231E6PM --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="timer.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

tm4c1231e6pm_startup_ccs.obj: ../tm4c1231e6pm_startup_ccs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/TivaWare_C_Series-2.1.2.111" --include_path="/include" -g --gcc --define=ccs="ccs" --define=PART_TM4C1231E6PM --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="tm4c1231e6pm_startup_ccs.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


