# Tutorial_generators
1) Instructions for installing Madgraph

		tar -xzf MG5_aMC_v2.9.2.tar.gz
	
		cd MG5_aMC_v2_9_2
  
		./bin/mg5_aMC
 
	(The first time you run Madgraph, you can be asked wether you want to update the software:
	New Version of MG5 available! Do you want to update your current version?[n, y, on_exit][60s to answer] 
	Answer >n)

	Now you are ready to generate processes


2) Instructions for installing Pythia

		MG5_aMC>install mg5amc_py8_interface
	
	
3) Generate the process mu mu -> H -> ZZ -> 4mu

	The Higgs boson is produced through WW fusion:
	mu+ mu- -> H vm vm~ -> Z Z* vm vm~ -> mu+ mu- mu+ mu- vm vm~

	In order to generate this process in Madgraph, we do:
		MG5_aMC>generate mu+ mu- > H vm vm~ , (H > Z mu+ mu- , Z > mu+ mu-)

	Save the process in the folder "signal"
		MG5_aMC>output signal

4) Feynman diagrams	
	We can see the Feynman diagrams of the generated process in the file:
	MG5_aMC_v2_9_2/prova1/SubProcesses/P1_ll_hvlvl_h_zll_z_ll/matrix1.ps

	2 diagrams have been generated

5) generate events
	In the folder MG5_aMC_v2_9_2/prova1/Cards several cards that define the parameters for the generation are created
	Open and modify the card run_card.dat
	We want to generate 1000 events with a center-of-mass energy of 1500 GeV 

	In Madgraph, execute the following command:
		MG5_aMC>launch prova1/

	In order to activate Pythia:
		>shower=Pythia8
		>done

	Then you are asked: Do you want to edit a card (press enter to bypass editing)? 
	press enter

	A html file will be opened in your browser

