#!/bin/bash
# -rt rebuild make file for new source code file test.
# -t test all UT.
# -rm rebuild make file for new source code file package

working_path=`dirname $0`
script_name=`basename $0`
complier="g++"
complie_flags="-g -Wall -DDO_COMPILE -D__STDC_CONSTANT_MACROS -fPIC -DSTACK_TRACE_FLAG -fno-threadsafe-statics -fno-enforce-eh-specs"
# -fno-exceptions -fno-rtti"
tmp_projects=`ls "$working_path" -F |grep "/$"`
projects=""
for project in $tmp_projects
do
	project=${project%%/*}
	source_count=`find $working_path/$project -name "*.cpp" | wc -l`
	if [ $source_count != 0 ]; then
		projects="$projects $project";
	fi
done

include_paths="-I ../"
lib_paths=""

function print_project_dependancy() {
	project_dependancy=""
	for src_file in `find ../$project -name "*.cpp"`
	do
		target_dependency=`g++ -MM $src_file $include_paths`
		for object in ${target_dependency##*:}
		do
			if [ $object != "\\" ]; then
				echo $object
			fi
		done
	done
}

function get_project_dependancy(){
	project_dependancy=""
	for object in `print_project_dependancy $0 | sort | uniq`
	do
		project_dependancy="$project_dependancy $object"
	done
	echo $project_dependancy
}

function create_make_for_project() {
## process c++
	cd $working_path/$1
	llib=""
	obj_list=""
	is_lib="true"
	file_libs=""
	local_lib_paths=""
	if [ -e libs ]; then
		file_libs=`cat libs`
		for lib in $file_libs
		do
			llib="$llib -l$lib"
		done
	fi
	> makefile
	for src_file in `find . -name "*.cpp"`
	do
		target_dependency=`g++ -MM $src_file $include_paths`
		target="Debug/${target_dependency%%:*}"
		obj_list="$obj_list $target"
		echo "Debug/$target_dependency" >> makefile
		echo -e "\t../$script_name -i \"\\\\e[31m\" \"\\\\e[0m\" $complier -c $src_file -o $target $include_paths $complie_flags" >> makefile
		if echo $src_file | grep -q "main.cpp"; then
			is_lib="false"
		fi
	done
	
	if echo $is_lib | grep -q "true"; then
		target="Debug/lib${1}.so"
		echo "$target: $obj_list" >> makefile
		for project in $projects
		do
			project=${project%%/*}
			if echo "$file_libs" | grep -q "$project"; then
				echo -e "\tmake -C ../$project Debug/lib$project.so && \\" >> makefile
				local_lib_paths="$local_lib_paths -L ../$project/Debug"
			fi
		done
		echo -e "\t../$script_name -i \"\\\\e[31m\" \"\\\\e[0m\" $complier -shared -o $target $obj_list $local_lib_paths $llib" >> makefile
		echo -e "\t[ -d ../libs ] || mkdir ../libs" >> makefile
		echo -e "\tcp $target ../libs" >> makefile
	else
		target="Debug/${1}"
		echo "$target: $obj_list" >> makefile
		for project in $projects
		do
			project=${project%%/*}
			if echo "$file_libs" | grep -q "$project"; then
				echo -e "\tmake -C ../$project Debug/lib$project.so && \\" >> makefile
				local_lib_paths="$local_lib_paths -L ../$project/Debug"
			fi
		done
		echo -e "\t../$script_name -i \"\\\\e[31m\" \"\\\\e[0m\" $complier -o $target $obj_list $lib_paths $local_lib_paths $llib" >> makefile
	fi
	echo ".PHONY clean:" >> makefile
	echo "clean:" >> makefile
	echo -e "\t rm -rf Debug/*.o $target" >> makefile
	cd - > /dev/null
}

function create_make_for_test_project() {
## process c++
	cd $working_path/$1
	target_project=${1/_test/}
	llib=""
	obj_list=""
	test_list=""
	test_table=""
	file_libs=""
	local_lib_paths=""
	local_lib_list=""
	if [ -e libs ]; then
		file_libs=`cat libs`
		for lib in $file_libs
		do
			llib="$llib -l$lib"
		done
	fi
	> makefile
	> ut.h
	
	for src_file in `find . -name "*.cpp"`
	do
		target_dependency=`g++ -MM $src_file $include_paths`
		target="Debug/${target_dependency%%:*}"
		obj_list="$obj_list $target"
		echo "Debug/$target_dependency" >> makefile
		echo -e "\t../$script_name -i \"\\\\e[31m\" \"\\\\e[0m\" $complier -c $src_file -o $target $include_paths $complie_flags -DTEST_MODE" >> makefile
		
		if echo $src_file | grep -vq "main.cpp"; then
			test_target="Debug/${target_dependency%%.*}.test"
			test_target_line="$test_target: ${target_dependency##*:}"
			#test_list="$test_list $test_target"
			
			head_file=${src_file%%cpp}
			head_file="${head_file:2}h"
			test_table_row="`grep $head_file ./main.cpp -Fn | grep \#include | awk -F: '{print $1}'` $test_target"
			test_table=`echo -e "$test_table_row\n$test_table"`

			if [ -e ../$target_project/${src_file/_test/} ]; then
				test_target_line="$test_target_line ../$target_project/${target/_test/}"
			fi

			echo "$test_target_line" >> makefile
			echo -e "\ttouch $test_target" >> makefile
			echo -e "\techo \"CPPUNIT_TEST_SUITE_REGISTRATION(${target_dependency%%.*}_class);\" >> ut.tmp" >> makefile
		fi
	done
	for test_target in `echo "$test_table" | sort -g -k1 | awk '{print $2}'`
	do
		test_list="$test_list $test_target"
	done
	
	for project in $projects
	do
		project=${project%%/*}
		if echo "$file_libs" | grep -q "^$project\$"; then
			local_lib_list="$local_lib_list ../$project/Debug/lib$project.so"
			local_lib_paths="$local_lib_paths -L ../$project/Debug"
			echo "../$project/Debug/lib$project.so: "`get_project_dependancy $project` >> makefile
			
			echo -e "\tmake -C ../$project Debug/lib$project.so" >> makefile
		fi
	done
	
	target="Debug/${1}"
	echo "$target: $local_lib_list $obj_list">> makefile
	
	echo -e "\t../$script_name -i \"\\\\e[31m\" \"\\\\e[0m\" $complier -o $target $obj_list $lib_paths $local_lib_paths $llib" >> makefile
	
	echo "Debug/test_case.test: ../$target_project/Debug/lib$target_project.so $test_list" >> makefile
	echo -e "\ttouch Debug/test_case.test" >> makefile
	echo -e "\t[ -e ut.tmp ] && cp -f ut.tmp ut.h" >> makefile
	
	echo ".PHONY clean_test:" >> makefile
	echo "clean_test:" >> makefile
	echo -e "\trm -rf Debug/*.test" >> makefile

	echo ".PHONY clean_obj:" >> makefile
	echo "clean_obj:" >> makefile
	echo -e "\trm -rf Debug/*.o $target" >> makefile
		
	echo ".PHONY clean:" >> makefile
	echo "clean:" >> makefile
	echo -e "\trm -rf Debug/*.o Debug/*.test $target" >> makefile
	cd - > /dev/null
}

case $1 in
	-m)
	for project in $projects
	do
		project=${project%%/*}
		if [ ! -d $working_path/$project/Debug ]; then
			mkdir -p $working_path/$project/Debug
		fi
		#lib_paths="$lib_paths -L ../$project/Debug/"
	done
	for project in $projects
	do
		project=${project%%/*}
		if echo "$project" | grep -q "_test$"; then
			#test project
			create_make_for_test_project $project
		else
			#none test project
			create_make_for_project $project
		fi
	done
	;;
	-ct)
	echo ">>>>>>>>>>>>>>>>>>>>>>CLEAN TEST CASE>>>>>>>>>>>>>>>>>>>>>>>>"
	for project in $projects
	do
		project=${project%%/*}
		echo ===================="$project"=====================
		if echo "$project" | grep -q "_test$"; then
			make -C $working_path/$project/ clean_test
		fi
	done
	;;
	-co)
	echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
	for project in $projects
	do
		project=${project%%/*}
		echo ===================="$project"=====================
		if echo "$project" | grep -q "_test$"; then
			make -C $working_path/$project/ clean_obj
		else
			make -C $working_path/$project/ clean
		fi
	done
	;;
	-c)
	echo ">>>>>>>>>>>>>>>>>>>>>CLEAN ALL>>>>>>>>>>>>>>>>>>>>>>>>>"
	for project in $projects
	do
		project=${project%%/*}
		echo ===================="$project"=====================
		make -C $working_path/$project/ clean 
	done
	;;
	-nt)
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$working_path/libs
	echo ">>>>>>>>>>>>>>>>>>>>>>>TEST CHANGE CASE>>>>>>>>>>>>>>>>>>>>>>>"
	for project in $projects
	do
		project=${project%%/*}
		if echo "$project" | grep -q "_test$"; then
			echo ===================="$project"=====================
			[ -e $working_path/$project/ut.tmp ] && rm $working_path/$project/ut.tmp
			make -C $working_path/$project/ Debug/test_case.test && \
			make -C $working_path/$project/ Debug/$project && \
			[ -e $working_path/$project/ut.tmp ] && \
			$working_path/$script_name -i "\\e[36m" "\\e[0m" $working_path/$project/Debug/$project
			if [ $? != 0 ]; then
				if [ -e $working_path/$project/ut.tmp ]; then
					break
				fi
			fi
		fi
	done
	;;
	-at)
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$working_path/libs
	echo ">>>>>>>>>>>>>>>>>>>>>>TEST ALL CASE>>>>>>>>>>>>>>>>>>>>>>>>"
	for project in $projects
	do
		project=${project%%/*}
		if echo "$project" | grep -q "_test$"; then
			echo ===================="$project"=====================
			[ -e $working_path/$project/ut.tmp ] && rm $working_path/$project/ut.tmp
			make -C $working_path/$project/ clean_test && \
			make -C $working_path/$project/ Debug/test_case.test && \
			make -C $working_path/$project/ Debug/$project && \
			$working_path/$script_name -i "\\e[36m" "\\e[0m" $working_path/$project/Debug/$project
			if [ $? != 0 ]; then
				if [ -e $working_path/$project/ut.tmp ]; then
					break
				fi
			fi
		fi
	done
	;;
	-t)
	echo ">>>>>>>>>>>>>>>>>>>>>>TEST AGAIN>>>>>>>>>>>>>>>>>>>>>>>>"
	for project in $projects
	do
		project=${project%%/*}
		if echo "$project" | grep -q "_test$"; then
			echo ===================="$project"=====================
			make -C $working_path/$project/ Debug/$project && \
			[ -e $working_path/$project/ut.tmp ] && \
			$working_path/$script_name -i "\\e[36m" "\\e[0m" $working_path/$project/Debug/$project
			if [ $? != 0 ]; then
				if [ -e $working_path/$project/ut.tmp ]; then
					break
				fi
			fi
		fi
	done
	;;
	-cm)
	echo ">>>>>>>>>>>>>>>>>>>>>>COMPILE ALL>>>>>>>>>>>>>>>>>>>>>>>>"
	for project in $projects
	do
		project=${project%%/*}
		echo ===================="$project"=====================
		if echo "$project" | grep -q "_test$"; then
			make -C $working_path/$project/ Debug/test_case.test
		fi
		if grep -q "^Debug/${project}:" $working_path/$project/makefile; then
			make -C $working_path/$project/ Debug/$project
		else
			make -C $working_path/$project/ Debug/lib${project}.so
		fi
		if [ $? != 0 ]; then
			break
		fi
	done
	;;
	-i)
		script_command=$0
		shift;
		str1=$1
		shift;
		str2=$1
		shift;
		set -o pipefail
		$@ 2>&1 |  $script_command -p "$str1" "$str2"
	;;
	-p)
		old_IFS=$IFS
		IFS="\n"
		while read line
		do
			printf "$2%s$3\n" "$line"
		done
		IFS=$old_IFS
	;;
	-ln)
		find . -name "*.h" -exec cat {} \; -or -name "*.cpp" -exec cat {} \; | wc -l
	;;
	-install)
		cp $working_path/libs/* /usr/local/lib/
		cp $working_path/simplayer/Debug/simplayer /usr/bin/
	;;
	-uninstall)
		for project in $projects
		do
			project=${project%%/*}
			if echo "$project" | grep -vq "_test$"; then
				rm -rf /usr/local/lib/lib$project.so
			fi
		done
		rm -rf /usr/bin/simplayer
	;;
	*)
		echo "-m create all projects makefile"
		echo "-ct clean all test."
		echo "-co clean all objs."
		echo "-c clean all."
		echo "-nt Do the new changed code's test."
		echo "-at Do all test."
		echo "-t Re test."
		echo "-cm Compile all."
		echo "-install/uninstall install/uninstall simplayer."
		exit -1;
	;;
	esac


