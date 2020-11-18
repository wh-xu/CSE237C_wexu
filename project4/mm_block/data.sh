data_csv=resource_tp.csv

rm $data_csv

for M_SIZE in 128 256 512 
do
	for BL in 4 8 16 32
	do
		for UR in 1 2 4 8
		do
            
            TP_a=$(grep -oP "Kernel throughput initializing a: [0-9]+[.][0-9]" ./results/TP_BMM_M_$M_SIZE\_BL_$BL\_UR_$UR.dat | grep -oP "[0-9]+[.][0-9]")
            TP_b=$(grep -oP "Kernel throughput initializing b: [0-9]+[.][0-9]" ./results/TP_BMM_M_$M_SIZE\_BL_$BL\_UR_$UR.dat | grep -oP "[0-9]+[.][0-9]")
            TP_c=$(grep -oP "Kernel throughput calculating c: [0-9]+[.][0-9]" ./results/TP_BMM_M_$M_SIZE\_BL_$BL\_UR_$UR.dat | grep -oP "[0-9]+[.][0-9]")
            
            rm -r ./matrix_mul_dpcpp.prj/
            unzip ./results/report_BMM_M_$M_SIZE\_BL_$BL\_UR_$UR.zip >> null.out
            
            resource=$(grep -oP "\"name\":\"c_calc\",\"total_kernel_resources\":.*?,\"total_percent\"" ./matrix_mul_dpcpp.prj/reports/lib/report_data.js | grep -oP "[0-9]+[0-9]")
                        
            sum=0; 
            for i in $resource;
            do 
                sum=$(expr $sum + $i); 
            done; 
            
            echo "Design space exploration: M_SIZE=$M_SIZE,\tBL=$BL,\tUR=$UR,\tSUM=$sum,\tTP=$TP_c"
            
            echo "$M_SIZE\t$BL\t$UR\t$sum\t$TP_c" >> $data_csv
		done
	done
done
