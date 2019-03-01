import pandas as pd, pylab as py, sys
import numpy as np
import os
import json
import math
from scipy import stats
from matplotlib.backends.backend_agg import FigureCanvasAgg as FigureCanvas
from matplotlib.figure import Figure
from matplotlib.backends.backend_pdf import PdfPages

def getRates(det,fill,scanfile):
	#root="/brildata/vdmoutput18/Analysed_Data/"
	fileName=scanfile+"/LuminometerData/"+"Rates_"+det+"_"+str(fill)+".json"
	print fileName
	table = {}
	calib=11245.5
	if det=="PLT":
		calib=calib/252.1
	elif det=="HFET":
		calib=calib/2431
	elif det=="HFOC":
		calib=calib/797.5
	elif det=="BCM1F":
		calib=calib/203.2
	elif det=="PCC":
		calib=calib/5910000*23.311

	with open(fileName, 'rb') as f:
		table = json.load(f)
	key = "Scan_" + str(1)
	rateData = table[key]
	lumiPerSP1 = [entry['Rates'] for entry in rateData]
	scanpoints=len(lumiPerSP1)
	print det, " Found ",scanpoints, " Scan Points"
	lumiErrPerSP1 = [entry['RateErrs'] for entry in rateData]
	key = "Scan_" + str(2)
	scan2=False
	try:
		rateData = table[key]
		lumiPerSP2 = [entry['Rates'] for entry in rateData]
		lumiErrPerSP2 = [entry['RateErrs'] for entry in rateData]
		scan2=True
	except:
		print "cannot find scan_2"

	usedCollidingBunches=[]
	lumiPerBX=[]
	lumiErrPerBX =[]
	for i in range(0,3400):
		for j in range(0,scanpoints):
			try:
				if str(i) in lumiPerSP1[j]:
					if i not in usedCollidingBunches:
						usedCollidingBunches.append(i)
				if scan2 and (str(i) in lumiPerSP2[j]):
					if i not in usedCollidingBunches:
						usedCollidingBunches.append(i)
			except:
				print "in usedCollidingBunches: BCID ", i, " is not filled at the scanpoint ", j
	
	lumi = [[] for a in range(len(usedCollidingBunches))]
	lumiErr = [[] for a in range(len(usedCollidingBunches))]
	for i, bx in enumerate(usedCollidingBunches):
		for j in range(0,scanpoints):
			try:
				value = lumiPerSP1[j][str(bx)]*calib
				lumi[i].append(value)
				valueErr = lumiErrPerSP1[j][str(bx)]*calib
				lumiErr[i].append(valueErr)
				if (bx==73) and (det=="HFET"):
					print "HFET scanpoint,value ",j,value
				if scan2:
					value = lumiPerSP2[j][str(bx)]*calib
					lumi[i].append(value)
					valueErr = lumiErrPerSP2[j][str(bx)]*calib
					lumiErr[i].append(valueErr)
			except:
				print "in GetLuminometerData: BCID ", bx, "is not filled at the scanpoint ", j

	return scanpoints,usedCollidingBunches,lumi,lumiErr


def main():
	scanfile = sys.argv[1]
	plotem="False"
	#plotem can have values missing=False, AllBx, TenBx, VersusBX, or combinations like AllVersusBx
	try:
		z=sys.argv[2]
		plotem=z
	except:
		plotem="False"
	output=open("linearityOverPCC.csv","a")
	fillreport=pd.read_csv('/nfshome0/stickzh/VdMFramework/Run2FillReport.csv',index_col='fill', sep='\t')
	fields=scanfile.split('/')
	filename=fields[len(fields)-1]
	fill =filename[0:4]
	print "fill ",fill
	detectors=["PLT","HFET","HFOC","BCM1F"]
	scanpoints,pccBunches,pcclumi, pcclumiErr = getRates("PCC",fill,scanfile)
	scanpoints,pltBunches,pltlumi, pltlumiErr = getRates("PLT",fill,scanfile)
	scanpoints,hfetBunches,hfetlumi, hfetlumiErr = getRates("HFET",fill,scanfile)
	scanpoints,hfocBunches,hfoclumi, hfoclumiErr = getRates("HFOC",fill,scanfile)
	scanpoints,pcvdBunches,pcvdlumi, pcvdlumiErr = getRates("BCM1F",fill,scanfile)
	#plotem=False
	leadgap=1
	pltresults=""
	hfetresults=""
	hfocresults=""
	pcvdresults=""

	figdir="/scratch/stickzh/linearity/"+filename
	if not os.path.exists(figdir):
		os.makedirs(figdir)

	print "pccBunches",pccBunches
	for det in detectors:
		pp = PdfPages(figdir+'/'+det+'_overPCC.pdf')
		leading = []
		train = []
		trainwgt = []
		leadingwgt = []
		leadingicept =[]
		trainicept = []
		lastbx=0
		grads = []
		graderr = []
		bxx = []
		gap =[]
		nbxDone=0
		
		for i, bx in enumerate(pccBunches):
			#if bx<570 or bx>625:
			#	continue
			#py.figure(figsize=(8,5))
			ratio = []
			ratioErr = []
			x =[]

			for j in range(0,len(pcclumi[i])):
				try:
					#print pcclumi[i][j]
					if pcclumi[i][j]>1.0:
						b=pcclumiErr[i][j]/pcclumi[i][j]
						if det=="HFET":
							r=hfetlumi[i][j]/pcclumi[i][j]
							a=hfetlumiErr[i][j]/hfetlumi[i][j]
						elif det=="HFOC":
							r=hfoclumi[i][j]/pcclumi[i][j]
							a=hfoclumiErr[i][j]/hfoclumi[i][j]
						elif det=="PLT":
							if (fill=='7358') and (j==0):
								continue
							r=pltlumi[i][j]/pcclumi[i][j]
							a=pltlumiErr[i][j]/pltlumi[i][j]
							if abs(pltlumiErr[i][j])<0.0001 or abs(pltlumi[i][j])<0.001:
								continue
						elif det=="BCM1F":
							r=pcvdlumi[i][j]/pcclumi[i][j]
							a=pcvdlumiErr[i][j]/pcvdlumi[i][j]
							if abs(pcvdlumiErr[i][j])<0.0001 or abs(pcvdlumi[i][j])<0.001:
								continue
						rerr=math.sqrt(r*r*(a*a+b*b))
						ratio.append(r)
						ratioErr.append(rerr)
						x.append(pcclumi[i][j])


				except Exception as e: print(e)
					#print 'failed for BX ',bx
			#print bx,len(hfoclumi[i]),len(ratio)
			if len(x)==len(ratio) and len(x)>5:
				gradient,intercept,r_value,p_value,std_err=stats.linregress(x,ratio)
				if ("Ten" in plotem and nbxDone<10) or ("All" in plotem) :
					nbxDone=nbxDone+1
					fig = Figure()
					canvas = FigureCanvas(fig)
					ax = fig.add_subplot(111)
					ax.errorbar(x, ratio, yerr=ratioErr, marker='s', fmt='o' )
					xmin, xmax = ax.get_xlim()
					ymin, ymax = ax.get_ylim()
					ax.grid()
					ax.plot([xmin, xmax], [intercept+gradient*xmin, intercept+gradient*xmax], color='r', linestyle='-', linewidth=3)
					ax.text(xmin+(xmax-xmin)*.75,ymin+(ymax-ymin)*.2,"slope = "+str(round(gradient*100,2))+"%"+"\n    +- "+str(round(std_err*100,2)),color="r", bbox=dict(facecolor='white', alpha=1.0))
					ax.set_ylabel(det+' SBIL/PCC SBIL')
					ax.set_title('SBIL '+det+'/PCC fill '+str(fill)+'  BX '+str(bx))
					ax.set_xlabel('PCC SBIL')
					pp.savefig(fig)
					fig.clf()
					py.close(fig)
					#print det,bx,len(x),round(gradient*100,3),round(std_err*100,3),xmin,xmax
				gap.append(bx-lastbx)
				if (bx-lastbx)==1:
					train.append(gradient)
					trainwgt.append(1/(std_err*std_err))
					trainicept.append(intercept)
					#print "Train\t",det,"\t",bx,"\t",bx-lastbx,"\t",gradient,"\t",std_err

				elif (bx-lastbx)>leadgap:
					leading.append(gradient)
					#print "Leading\t",det,"\t",bx,"\t",bx-lastbx,"\t",gradient,"\t",std_err
					leadingwgt.append(1/(std_err*std_err))
					leadingicept.append(intercept)
				grads.append(gradient)
				graderr.append(std_err)
				bxx.append(bx)
				lastbx=bx

		nbx=len(bxx)
		nlead=len(leading)
		ntrain=len(train)
		#print "Means ",np.mean(leading), np.average(leading), np.average(leading,weights=leadingwgt)
		if det=="PLT":
			pltresults=pltresults\
			+str(round(np.average(leading,weights=leadingwgt),5))+","\
			+str(round(np.std(leading),5))+","\
			+str(round(np.mean(leadingicept),3))+","
			if ntrain>0:
				pltresults=pltresults\
				+str(round(np.average(train,weights=trainwgt),5))+","\
				+str(round(np.std(train),5))+","\
				+str(round(np.mean(trainicept),3))+","
			else:
				pltresults=pltresults+",,,"
		elif det=="HFET":
			hfetresults=hfetresults\
			+str(round(np.average(leading,weights=leadingwgt),5))+","\
			+str(round(np.std(leading),5))+","\
			+str(round(np.mean(leadingicept),3))+","
			if ntrain>0:
				hfetresults=hfetresults\
				+str(round(np.average(train,weights=trainwgt),5))+","\
				+str(round(np.std(train),5))+","\
				+str(round(np.mean(trainicept),3))+","
			else:
				hfetresults=hfetresults+",,,"
		elif det=="BCM1F":
			pcvdresults=pcvdresults\
			+str(round(np.average(leading,weights=leadingwgt),5))+","\
			+str(round(np.std(leading),5))+","\
			+str(round(np.mean(leadingicept),3))+","
			if ntrain>0:
				pcvdresults=pcvdresults\
				+str(round(np.average(train,weights=trainwgt),5))+","\
				+str(round(np.std(train),5))+","\
				+str(round(np.mean(trainicept),3))+","
			else:
				hfetresults=hfetresults+",,,"
		elif det=="HFOC":
			hfocresults=hfocresults\
			+str(round(np.average(leading,weights=leadingwgt),5))+","\
			+str(round(np.std(leading),5))+","\
			+str(round(np.mean(leadingicept),3))+","
			if ntrain>0:
				hfocresults=hfocresults\
				+str(round(np.average(train,weights=trainwgt),5))+","\
				+str(round(np.std(train),5))+","\
				+str(round(np.mean(trainicept),3))+","
			else:
				hfocresults=hfocresultst+",,,"			

	
		if nlead >1:
			print det+" Average Leading Slope =",round(np.average(leading,weights=leadingwgt),5),"   +- ",round(np.std(leading),5)
		if ntrain>1:
			print det+" Average Train Slope =",round(np.average(train,weights=trainwgt),5),"   +- ",round(np.std(train),5)	
		

		if "Versus" in plotem:			
			fig,ax =py.subplots()
			ax.errorbar(bxx, grads, yerr=graderr, marker='s', fmt='o' )
			ax.set_ylabel(det+' Gradient wrt PCC')
			ax.grid()
			if fill=='7358':
				ax.set_xlim(1640,1660)
			ax.set_title('Gradient '+det+' wrt PCC fill '+str(fill))
			ax.set_xlabel('BX')
			#py.show()
			#fig.savefig(figdir+'/'+det+'allBX'".png")
			pp.savefig(fig)
			py.close(fig)
			if fill=='7358':
				fig,ax =py.subplots()
				ax.errorbar(bxx, grads, yerr=graderr, marker='s', fmt='o' )
				ax.set_ylabel(det+' Gradient wrt PCC')
				ax.grid()
				if fill=='7358':
					ax.set_xlim(745,765)
				ax.set_title('Gradient '+det+' wrt PCC fill '+str(fill))
				ax.set_xlabel('BX')
				pp.savefig(fig)
				py.close(fig)

			fig,ax =py.subplots()
			ax.errorbar(gap, grads, yerr=graderr, marker='s', fmt='o' )
			ax.set_ylabel(det+' Gradient wrt PCC')
			ax.set_title('Gradient '+det+' wrt PCC fill '+str(fill))

			
			ax.set_xlabel('Gap before this BX')
			#py.show()
			#fig.savefig(figdir+'/'+det+'gapBX'".png")
			pp.savefig(fig)
		
		if plotem != "False":
			pp.close()		
			py.close(fig)


	header=scanfile+","+str(fill)+","\
	+str(fillreport.loc[int(fill),'ilumi'])+","\
	+str(fillreport.loc[int(fill),'run1'])+","\
	+str(nbx)+","+str(nlead)+","+str(ntrain)+","+str(scanpoints)+","
	output.write(header+pltresults+hfetresults+pcvdresults+hfocresults+"\n")

	if plotem != "False":
		print "plots sent to ", figdir





if __name__ == "__main__":
    main()
