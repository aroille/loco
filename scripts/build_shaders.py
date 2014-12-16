
# script parameters :
shader_path = "samples\\resources\\loco\\shader\\"
include_path = ["3rdparty\\bgfx\\src\\", "samples\\resources\\loco\\shader\\common\\"]
app_path = "..\\bgfx\\bgfx\\tools\\bin\\windows\\shaderc.exe"

backend_vs= []
backend_vs.append({'platform': 'windows', 'profile': 'vs_3_0', 'extention': 'ader_dx9'})
backend_vs.append({'platform': 'windows', 'profile': 'vs_4_0', 'extention': 'ader_dx11'})

backend_ps= []
backend_ps.append({'platform': 'windows', 'profile': 'ps_3_0', 'extention': 'ader_dx9'})
backend_ps.append({'platform': 'windows', 'profile': 'ps_4_0', 'extention': 'ader_dx11'})



# start script
import os
import glob
import subprocess
import time

root_path = os.path.dirname(os.path.realpath(__file__)) + "\\..\\"
os.chdir(root_path + shader_path)

# shaderc -i parameter
include_full_path = ""
for path in include_path:
	include_full_path = include_full_path + root_path + path + ";"

# clean directory
vs_filenames = glob.glob("vs_*.sh")
ps_filenames = glob.glob("ps_*.sh")

for be in backend_ps:
	for file in vs_filenames:
		try:
			os.remove(file + be['extention'])
		except:
			pass
			
	for file in ps_filenames:
		try:
			os.remove(file + be['extention'])
		except:
			pass

# avoid loco to hot-reload only the non-existing file		
time.sleep(0.03)	

# generate vertex shaders
for file in vs_filenames:
	print file
	for be in backend_vs:
		varying_def = 'va_' + file[3:]
		output =  file + be['extention']
		output_tmp = output + '.tmp'
		subprocess.call([root_path+app_path, '--type', 'v', '--platform',  be['platform'], '-p', be['profile'], '-f', file, '-o', output_tmp, '-i', include_full_path, '--varyingdef', varying_def])
		os.rename(output_tmp, output)
	
# generate pixel shaders
for file in ps_filenames:
	print file
	for be in backend_ps:
		varying_def = 'va_' + file[3:]
		output =  file + be['extention']
		output_tmp = output + '.tmp'
		subprocess.call([root_path+app_path, '--type', 'f', '--platform',  be['platform'], '-p', be['profile'], '-f', file, '-o', output_tmp, '-i', include_full_path, '--varyingdef', varying_def])
		os.rename(output_tmp, output)