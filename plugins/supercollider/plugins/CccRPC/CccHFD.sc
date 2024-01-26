CccHFD : UGen {
	*ar { |input, winSize=250, hopSize=0.5, kMax=10, maxWinSize=500|
		^this.multiNew('audio', input, winSize, hopSize, kMax, maxWinSize);
	}
	checkInputs {
		var maxWinSize = inputs.at(4);
 		if (maxWinSize.rate != 'scalar', {
 			^("maxWinSize in not modulateable, please use a scalar value: " + maxWinSize + maxWinSize.rate);
 		});

		^this.checkValidInputs;
	}
}

