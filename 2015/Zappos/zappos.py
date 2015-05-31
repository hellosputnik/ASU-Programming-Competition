# Following in Turing's Footsteps

# TO-DO:

# Refactor code decrypt function to handle notches. We currently do it manually
# in the main function.

from collections import deque

# Our bit string
string = ""

# Read the file as a binary file
with open("asu_challenge.bmp", 'rb') as f:
    # Read the file as a byte array
    byte = bytearray(f.read())
    for each in byte:
        # Add the LSB to the bit string
        string += str(each & 1)

string = string[54:]        # The first 54 bytes are header bytes
                            # Skip the first 54 bits of our bit string

length = string[:32]        # The length of the payload is 4 bytes (i.e. 32 bits)
                            # 4-byte integer, not literally 4 bytes

payload = string[32:-24]    # The payload starts at the end of the length up to 
                            # the last 3 bytes

EOL = string[-24:]          # The EOL string is the last 3 bytes

# es is short for encrypted string
es = ""

# Convert the payload
for x in xrange(515):
    # Convert to ASCII character
    es += chr(int(payload[(8 * x):(8 * x + 8)], 2))

# Enigma machine parts
sw  = es[0:94]              # Static wheel                   (94 characters)
r   = es[94:94+94]          # Reflector                      (47 pairs of characters)
r1  = es[188:188+94]        # Rotor I                        (94 characters)
r2  = es[282:282+94]        # Rotor II                       (94 characters)
r3  = es[376:376+94]        # Rotor III                      (94 characters)
r1n = es[470:470+1]         # Rotor I Notch                  (1 character)
r2n = es[471:471+1]         # Rotor II Notch                 (1 character)
r1s = es[472:472+1]         # Rotor I Starting Position      (1 character)
r2s = es[473:473+1]         # Rotor II Starting Position     (1 character)
r3s = es[474:474+1]         # Rotor III Starting Position    (1 character)
emd = es[475:-3]            # Encrypted message data

# Rotor step as defined in the specifications (i.e. shift)
# c is a character and s is the iterable
def step(c, s):
    # Rotate, or "shift", characters
	d = deque(s)
	d.rotate(0 - s.index(c));

    # Return as a string
	return ''.join(d)

# Set initial configurations
r1 = step(r1s, r1)
r2 = step(r2s, r2)
r3 = step(r3s, r3)

# Debug operations
# print sw + '\n'
# print r  + '\n'
# print r1 + '\n'
# print r2 + '\n'
# print r3 + '\n'

# Example on the specification site:
# sw = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
# r  = "HQ RE WS LV GY MA FO DJ CX PU ZT IK BN".replace(" ", "")
# r1 = "MOYWDLZCFHVXQATPEKUJNGBRSI"
# r2 = "VQZBUMWKRITEAHFPGSCNJYDLOX"
# r3 = "PHNOXSAYIDCQVMEJRUTZGKBLWF"

# Decrypt function
def decrypt(character):
    # Run through rotator 1
	result = r1[sw.index(character)]
	# print result

    # Run through rotator 2
	result = r2[sw.index(result)]
	# print result

    # Run through rotator 3
	result = r3[sw.index(result)]
	# print result

    # Run through reflector
	i = r.index(result)
	i = (i - 1) if (i % 2) else (i + 1)   # (i - 1) if odd else (1 + 1)
	result = r[i]
	# print result

    # Run through rotator 3
	result = sw[r3.index(result)]
	# print result

    # Run through rotator 2
	result = sw[r2.index(result)]
	# print result

    # Run through rotator 1
	result = sw[r1.index(result)]
	# print result

    # Return result!
	return result

def main():
    # Global variables
    global r1, r2, r3, r1n, r2n

    # Result string
    result = ""
    
    # For each character in the encrypted message data, run decrypt
    for character in emd:
        # Save the notch positions
    	n1, n2 = r1[0], r2[0]
    
        # Rotor 1 is always rotated
    	r1 = step(r1[-1], r1)
    
        # Step rotator 2 if notch of rotator 1 is aligned
    	if r1n == n1:
    		r2 = step(r2[-1], r2)
    
        # Step rotator 3 if notch of rotator 2 is aligned
    	if r2n == n2:
    		r3 = step(r3[-1], r3)
    
        # Add decrypted character to the result string
    	result += decrypt(character)
    
    # Print the result and celebrate
    print result

if __name__ == '__main__':
    main()

