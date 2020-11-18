for M_SIZE in 128 256 512 
do
	for BL in 4 8 16 32
	do
		for UR in 1 2 4 8
		do
            echo "Design space exploration: M_SIZE=$M_SIZE,\tBL=$BL,\tUR=$UR"
            
            cd src
			rm -rf matrix_mul_dpcpp.prj/ matrix_mul_dpcpp.o matrix_mul_dpcpp.a
            cd ..
            
			sed -i "s/^constexpr int m_size = .*$/constexpr int m_size = $M_SIZE*8;/" ./src/matrix_mul_dpcpp.cpp 
            sed -i "s/^constexpr int BL = .*$/constexpr int BL = $BL;/" ./src/matrix_mul_dpcpp.cpp 
            sed -i "s/^              #pragma unroll .*$/              #pragma unroll $UR/" ./src/matrix_mul_dpcpp.cpp
            
			make all
			make run > ./results/TP_BMM_M_$M_SIZE\_BL_$BL\_UR_$UR.dat

			cd src
            
			dpcpp -fintelfpga matrix_mul_dpcpp.cpp -c -o matrix_mul_dpcpp.o
			dpcpp -fintelfpga matrix_mul_dpcpp.o -fsycl-link -Xshardware

			zip -r ../results/report_BMM_M_$M_SIZE\_BL_$BL\_UR_$UR.zip ./matrix_mul_dpcpp.prj/ > null.out
            
            cd ..
		done
	done
done
