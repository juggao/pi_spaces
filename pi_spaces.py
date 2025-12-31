import math
from mpmath import mp

# ANSI color codes for terminal output
COLORS = {
    '2': '\033[91m',  # Red
    '3': '\033[93m',  # Orange/Yellow
    '4': '\033[92m',  # Green
    '5': '\033[94m',  # Blue
    '6': '\033[95m',  # Indigo/Magenta
    '7': '\033[96m',  # Violet/Cyan
    '8': '\033[97m',  # White
    'reset': '\033[0m'
}

def find_max_consecutive_spaces(text):
    """Find the maximum consecutive space length in text."""
    max_spaces = 0
    current_spaces = 0
    
    for char in text:
        if char == ' ':
            current_spaces += 1
            max_spaces = max(max_spaces, current_spaces)
        else:
            current_spaces = 0
    
    return max_spaces

def process_pi_until_target(target_spaces=10, chunk_size=1000, max_digits=1000000):
    """Process PI digits until finding target consecutive spaces."""
    print(f"Searching for {target_spaces} consecutive spaces (9s or 0s)...")
    print(f"Processing in chunks of {chunk_size} digits...\n")
    
    n = chunk_size
    found = False
    max_found = 0
    position = -1
    
    while n <= max_digits and not found:
        # Set precision high enough to get N digits
        mp.dps = n + 10
        pi_str = str(mp.pi).replace('.', '')[:n]
        
        # Replace 9 and 0 with spaces
        replaced = pi_str.replace('9', ' ').replace('0', ' ')
        
        # Check for consecutive spaces
        current_spaces = 0
        for i, char in enumerate(replaced):
            if char == ' ':
                current_spaces += 1
                if current_spaces >= target_spaces:
                    found = True
                    position = i - target_spaces + 2  # Position where sequence starts (1-indexed)
                    max_found = current_spaces
                    break
            else:
                if current_spaces > max_found:
                    max_found = current_spaces
                current_spaces = 0
        
        if found:
            print(f"✓ Found {target_spaces}+ consecutive spaces at position {position}!")
            print(f"  Total digits processed: {n}")
            print(f"  Maximum consecutive spaces found: {max_found}")
            return n, pi_str, replaced, position
        else:
            print(f"  Processed {n} digits... max consecutive spaces so far: {max_found}")
            n += chunk_size
    
    if not found:
        print(f"\n✗ Did not find {target_spaces} consecutive spaces in first {max_digits} digits")
        print(f"  Maximum consecutive spaces found: {max_found}")
        # Return what we have
        mp.dps = n + 10
        pi_str = str(mp.pi).replace('.', '')[:n]
        replaced = pi_str.replace('9', ' ').replace('0', ' ')
        return n, pi_str, replaced, -1
    
def colorize_and_save(text, output_file='pi_colored.txt'):
    """Color digits 2-7 in rainbow colors, 8 in white, save to file and display."""
    colored_text = ""
    plain_text = ""
    
    for char in text:
        if char in COLORS:
            colored_text += COLORS[char] + char + COLORS['reset']
            plain_text += char
        elif char == ' ':
            colored_text += ' '
            plain_text += ' '
        else:
            colored_text += char
            plain_text += char
    
    # Save plain text with spaces to file
    with open(output_file, 'w') as f:
        f.write(plain_text)
    
    print("\nColorized display (first 500 characters):")
    print(colored_text[:500])
    if len(colored_text) > 500:
        print("...")
    
    print(f"\nSaved colored digits (with spaces) to {output_file}")
    
    return colored_text

def main():
    print("=" * 60)
    print("PI DIGITS PROCESSOR - SEARCH FOR 10 CONSECUTIVE SPACES")
    print("=" * 60)
    print()
    
    # Process PI until we find 10 consecutive spaces
    n, pi_digits, replaced_text, position = process_pi_until_target(
        target_spaces=10, 
        chunk_size=5000,
        max_digits=1000000
    )
    
    # Save original PI digits
    with open('pi_digits.txt', 'w') as f:
        f.write(pi_digits)
    print(f"\nStep 1: Saved {n} digits of PI to pi_digits.txt")
    
    # Save replaced text
    with open('pi_replaced.txt', 'w') as f:
        f.write(replaced_text)
    print(f"Step 2: Saved replaced text to pi_replaced.txt")
    
    # Colorize, display and save
    print(f"\nStep 3: Colorizing and saving...")
    colorize_and_save(replaced_text)
    
    print("\n" + "=" * 60)
    print("Processing complete!")
    print("=" * 60)
    print(f"\nFiles created:")
    print("- pi_digits.txt (original {n} PI digits)")
    print("- pi_replaced.txt (with 9 and 0 replaced by spaces)")
    print("- pi_colored.txt (colored digits with spaces)")
    
    if position > 0:
        print(f"\n★ 10 consecutive spaces found at position {position}!")

if __name__ == "__main__":
    main()
