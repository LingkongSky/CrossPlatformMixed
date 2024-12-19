extract_dir="EXTRACTDIR"
target_file_prefix="TARGETPREFIX"
target_file_name="TARGETNAME"
target_file_md5="TARGETMD5"



if [ ! -d $extract_dir ];then
  mkdir $extract_dir
fi



for file in ${extract_dir}"/"${target_file_prefix}*; do
  if [ -f "$file" ]; then
    file_md5=$(echo "$file" | awk -F"${target_file_prefix}" '{print $2}')
    if [ "$file_md5" != "$target_file_md5" ]; then
      rm "$file"
    fi
  fi
done



if [ ! -f ${extract_dir}"/"${target_file_name} ];then
    sed "1,/^### END OF THE SCRIPT###/d" "$0" > ${extract_dir}"/"${target_file_name}
    chmod u+x ${extract_dir}"/"${target_file_name}
fi

# exec
${extract_dir}"/"${target_file_name}

exit;
### END OF THE SCRIPT###