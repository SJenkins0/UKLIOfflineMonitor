
exec=$homeDIR/monitoring/top_diffuser/main

for num in {82800..82960}; do
    if [[ -f "top_diffuser_0$num.dat" ]]; then
	mv -v top_diffuser_0$num.dat /disk02/usr6/jmcelwee/monitoringData/extDat/0$num/ 
    fi
done
