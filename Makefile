buildlib:
	cd java && javac com/graalvmfaas/GraalvmFaasBase.java && native-image --shared -H:Name=libgraalvmfaas

cleanlib:
	cd java && rm -rf `ls | grep -v libgraalvmfaas.dylib | grep -v com`

.PHONY: buildlib cleanlib