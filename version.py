import subprocess

try:
    v = subprocess.check_output(['git', 'describe', '--dirty', '--abbrev'], stderr=subprocess.DEVNULL).decode().strip()
    if '-' in v:
        bv = v[:v.index('-')]
        bv = bv[:bv.rindex('.') + 1] + str(int(bv[bv.rindex('.') + 1:]) + 1)
        sempre = 'dirty' if v.endswith('-dirty') else 'commit'
        build = subprocess.check_output(['git', 'rev-parse', '--short', 'HEAD']).decode().strip()
        semver = bv + '-' + sempre + '+' + build
    else:
        semver = v

    with open('version', 'w') as f:
        print('Semantic version is ' + semver)
        f.write(semver)
except subprocess.CalledProcessError as e:
    print('Error calling git', e)
