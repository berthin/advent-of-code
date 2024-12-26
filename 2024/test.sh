# File containing test cases
test_cases_file="day10.combi"
# The program to execute
program="./a.out"

# Open a file descriptor (FD 3) and run the program in the background
exec 3> >( $program )

max_tests=${1:-0}
test_count=0

# Read the test cases file
while IFS= read -r line || [[ -n $line ]]; do
  if [[ -z $line ]]; then
    # Blank line detected, close and reopen the file descriptor
    if [[ $max_tests -eq 0 || $test_count -lt $max_tests ]]; then
      exec 3>&-
      exec 3> >( $program )
    else
      break
    fi


    ((test_count++))

  else
    # Write non-blank line to the program's stdin
    echo "$line" >&3
  fi
done < "$test_cases_file"

# Close the file descriptor
exec 3>&-