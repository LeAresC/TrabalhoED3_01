#!/usr/bin/env python3

import subprocess
import sys
import os
from pathlib import Path

class Colors:
    GREEN = '\033[92m'
    RED = '\033[91m'
    YELLOW = '\033[93m'
    BLUE = '\033[94m'
    RESET = '\033[0m'
    BOLD = '\033[1m'

def normalize_line_endings(text):
    return text.replace('\r\n', '\n').replace('\r', '\n')

def read_file(filepath):
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            return normalize_line_endings(f.read())
    except FileNotFoundError:
        print(f"{Colors.RED}Error: File not found: {filepath}{Colors.RESET}")
        return None
    except Exception as e:
        print(f"{Colors.RED}Error reading {filepath}: {e}{Colors.RESET}")
        return None

def show_diff(expected_lines, actual_lines, max_lines=5):
    print(f"\n{Colors.YELLOW}First differences:{Colors.RESET}")
    
    max_len = max(len(expected_lines), len(actual_lines))
    diff_count = 0
    
    for i in range(min(max_len, max_lines)):
        expected_line = expected_lines[i] if i < len(expected_lines) else "<missing>"
        actual_line = actual_lines[i] if i < len(actual_lines) else "<missing>"
        
        if expected_line != actual_line:
            diff_count += 1
            print(f"\n  Line {i + 1}:")
            print(f"    {Colors.GREEN}Expected:{Colors.RESET} {repr(expected_line)}")
            print(f"    {Colors.RED}Actual:  {Colors.RESET} {repr(actual_line)}")
    
    if len(expected_lines) != len(actual_lines):
        print(f"\n  {Colors.YELLOW}Line count mismatch:{Colors.RESET}")
        print(f"    Expected: {len(expected_lines)} lines")
        print(f"    Actual:   {len(actual_lines)} lines")

def run_test(test_num, program_path='./programTrab'):
    input_file = f"{test_num}.in"
    output_file = f"{test_num}.out"
    
    # Check if files exist
    if not os.path.exists(input_file):
        print(f"{Colors.YELLOW}[SKIP]{Colors.RESET} Test {test_num}: Input file not found")
        return None
    
    if not os.path.exists(output_file):
        print(f"{Colors.YELLOW}[SKIP]{Colors.RESET} Test {test_num}: Output file not found")
        return None
    
    # Read input
    input_data = read_file(input_file)
    if input_data is None:
        return False
    
    # Read expected output
    expected_output = read_file(output_file)
    if expected_output is None:
        return False
    
    # Run the program
    try:
        result = subprocess.run(
            [program_path],
            input=input_data,
            capture_output=True,
            text=True,
            timeout=10
        )
        
        actual_output = normalize_line_endings(result.stdout)
        
        # Compare outputs
        if actual_output == expected_output:
            print(f"{Colors.GREEN}{Colors.BOLD}[PASS]{Colors.RESET} Test {test_num}")
            return True
        else:
            print(f"{Colors.RED}{Colors.BOLD}[FAIL]{Colors.RESET} Test {test_num}")
            
            # Show diff
            expected_lines = expected_output.split('\n')
            actual_lines = actual_output.split('\n')
            show_diff(expected_lines, actual_lines)
            
            # Show stderr if any
            if result.stderr:
                print(f"\n{Colors.RED}Program stderr:{Colors.RESET}")
                print(result.stderr)
            
            return False
            
    except subprocess.TimeoutExpired:
        print(f"{Colors.RED}[TIMEOUT]{Colors.RESET} Test {test_num}: Program took too long")
        return False
    except FileNotFoundError:
        print(f"{Colors.RED}[ERROR]{Colors.RESET} Program not found: {program_path}")
        print(f"Please compile first with: make")
        return False
    except Exception as e:
        print(f"{Colors.RED}[ERROR]{Colors.RESET} Test {test_num}: {e}")
        return False

def main():
    print(f"\n{Colors.BLUE}{Colors.BOLD}=== Graph Project Test Runner ==={Colors.RESET}\n")
    
    if not os.path.exists('./programTrab'):
        print(f"{Colors.RED}Error: programTrab not found!{Colors.RESET}")
        print(f"Please compile first with: {Colors.BOLD}make{Colors.RESET}\n")
        return 1
    
    test_range = range(1, 11)
    results = {}
    
    for test_num in test_range:
        result = run_test(f"test/{test_num}")
        if result is not None:
            results[test_num] = result
        print()
    
    if results:
        passed = sum(1 for r in results.values() if r)
        total = len(results)
        
        print(f"{Colors.BLUE}{Colors.BOLD}=== Summary ==={Colors.RESET}")
        print(f"Passed: {Colors.GREEN}{passed}{Colors.RESET}/{total}")
        print(f"Failed: {Colors.RED}{total - passed}{Colors.RESET}/{total}")
        
        if passed == total:
            print(f"\n{Colors.GREEN}{Colors.BOLD}All tests passed! 🎉{Colors.RESET}\n")
            return 0
        else:
            print(f"\n{Colors.RED}{Colors.BOLD}Some tests failed{Colors.RESET}\n")
            return 1
    else:
        print(f"{Colors.YELLOW}No tests were run{Colors.RESET}\n")
        return 1

if __name__ == '__main__':
    sys.exit(main())
