import ROOT
from array import array

def read_data(filename):
    data = {
        'run_number': [],
        'lumi_section': [],
        'pcc_count': [],
        'hfet_count': []
    }
    
    with open(filename, 'r') as file:
        for line in file:
            tokens = line.split()
            data['run_number'].append(int(tokens[0]))
            data['lumi_section'].append(int(tokens[1]))
            data['pcc_count'].append(float(tokens[2]))
            data['hfet_count'].append(float(tokens[3]))
    
    return data

def create_histograms(data):
    n = len(data['lumi_section'])
    bin_count = 150000 // 50  

    histogram_pcc_hfet = ROOT.TH2F("histogram_pcc_hfet", "PCC/HFET vs Lumi Section;Lumi Section;PCC/HFET", bin_count, 0, 150000, bin_count, 0.96, 1.04)

    x_values = []
    y_values = []

    histogram_pcc_vs_hfet = ROOT.TH2F("histogram_pcc_vs_hfet", "Integrated PCC/HFET vs Integrated HFET;PCC/HFET per 50 LS;Integrated lumi", 
                                      100, 0.96, 1.04, 100, 0, 1.5) 
    
    for i in range(n):
        hfet = data['hfet_count'][i]
        pcc = data['pcc_count'][i]

        ratio_pcc_hfet = pcc / hfet if hfet != 0 else 0
        histogram_pcc_hfet.Fill(i, ratio_pcc_hfet)

        if i % 50 == 0 and i != 0:  
            x_values.append(ratio_pcc_hfet)
            y_value = sum(data['hfet_count'][i-49:i+1]) / 1000000
            y_values.append(y_value)

            histogram_pcc_vs_hfet.Fill(ratio_pcc_hfet, y_value)

    graph_pcc_hfet = ROOT.TGraph(len(x_values), array('d', x_values), array('d', y_values))
    graph_pcc_hfet.SetTitle("Integrated PCC/HFET vs Integrated HFET;Integrated PCC/HFET;Integrated HFET")
    graph_pcc_hfet.SetMarkerStyle(20)

    return histogram_pcc_hfet, graph_pcc_hfet, histogram_pcc_vs_hfet, x_values, y_values

filename = "/eos/user/a/asehrawa/merged_ls_2022.dat"
data = read_data(filename)
histogram_pcc_hfet, graph_pcc_hfet, histogram_pcc_vs_hfet, x_values, y_values = create_histograms(data)

canvas_hist_pcc_hfet = ROOT.TCanvas("canvas_hist_pcc_hfet", "PCC/HFET vs Lumi Section", 1200, 800)
histogram_pcc_hfet.Draw()
canvas_hist_pcc_hfet.SaveAs("/eos/user/a/asehrawa/BRIL-new/histogram_pcc_hfet_1.png")

canvas_graph_pcc_hfet = ROOT.TCanvas("canvas_graph_pcc_hfet", "Integrated PCC/HFET vs Integrated HFET", 1200, 800)
graph_pcc_hfet.GetXaxis().SetRangeUser(0.96, 1.04)
graph_pcc_hfet.Draw("AP")
canvas_graph_pcc_hfet.SaveAs("/eos/user/a/asehrawa/BRIL-new/graph_pcc_hfet_1.png")

ROOT.gStyle.SetPalette(ROOT.kBird)  
histogram_pcc_vs_hfet.SetFillColor(ROOT.kRed)   
histogram_pcc_vs_hfet.SetLineColor(ROOT.kRed)  
canvas_hist_pcc_vs_hfet = ROOT.TCanvas("canvas_hist_pcc_vs_hfet", "Integrated PCC/HFET vs Integrated HFET", 1200, 800)
histogram_pcc_vs_hfet.Draw("COLZ BOX")
canvas_hist_pcc_vs_hfet.SaveAs("/eos/user/a/asehrawa/BRIL-new/histogram_pcc_vs_hfet_1.png")

