import yaml

binFile = open("output.rom", "wb")

stream = open('test.yml', 'r')

loaded = yaml.safe_load(stream)

keyss = loaded.get('rom_strings')

for key, value in keyss.items():
    binFile.write(bytearray(key, 'utf8'))
    binFile.write((value).to_bytes(8, 'little'))

print(keyss)

print(yaml.dump(loaded))


# check for the header key, stringlookup
# extract each key and value
# write key and value to a rom file
