float abreuZero = 0.0;

float abreuGains[12] =
{0.9871,0.8687,1.2614,0.8319,0.6877,1.3285,
        0.1878,0.1710,0.4028,0.5192,0.2816,0.6744};

//    int abreuMap[8] = {0,1,2,3,4,5,6,7};

//int abreuMap[8] = {0,1,2,3,9,10,6,11};
int abreuMap[12] = {0,1,2,3,4,5,6,7,8,9,10,11};

/*  
Power and root power in file abreu1.snd is 1289769.141135 and 1135.680035
Power and root power in file abreu2.snd is 816148.381467 and 903.409310
Power and root power in file abreu3.snd is 1853068.554244 and 1361.274606
Power and root power in file abreu4.snd is 623226.921145 and 789.447225
Power and root power in file abreu5.snd is 782737.309895 and 884.724426
Power and root power in file abreu6.snd is 805818.099196 and 897.673715
Power and root power in file abreu7.snd is 57629.795831 and 240.062067
Power and root power in file abreu8.snd is 55612.454043 and 235.822929
Power and root power in file abreu9.snd is 122417.507785 and 349.882134
Power and root power in file abreu10.snd is 319976.249874 and 565.664432
Power and root power in file abreu11.snd is 158841.666248 and 398.549453
Power and root power in file abreu12.snd is 654906.058513 and 809.262663
*/


float abreuRef[12] =
{1135.680035, 903.409310, 1361.274606, 789.447225, 884.724426, 897.673715,
    240.062067, 235.822929, 349.882134, 565.664432, 398.549453, 809.262663};


float abreu_coeffs[12][12] = {
{-1.483397, 1.331792, -1.084255, 0.946160, -1.023857, 0.965593, 
	-0.871034, 0.574233, -0.513980, 0.618198, -0.546657, 0.199459}, 
{-1.281295, 1.155020, -1.051618, 0.952715, -0.926322, 0.811729, 
	-0.823626, 0.610312, -0.551947, 0.486091, -0.363000, 0.143466}, 
{-1.322385, 1.480377, -1.253767, 0.817593, -0.789725, 0.694582, 
	-0.732815, 0.696744, -0.591191, 0.389738, -0.264039, 0.061338}, 
{-1.384671, 1.112362, -0.858544, 0.706595, -0.747668, 0.840537, 
	-0.777678, 0.453896, -0.325904, 0.385620, -0.360574, 0.131293}, 
{-1.499389, 1.512580, -1.528361, 1.377705, -1.077274, 0.889522, 
	-0.848521, 0.689615, -0.610641, 0.475703, -0.323393, 0.067883},
{-1.016416, 0.822849, -0.720991, 0.365314, -0.303139, 0.505000, 
	-0.544981, 0.477443, -0.419148, 0.221638, -0.035429, -0.147376}, 
{-1.701636, 1.709395, -1.522403, 1.112192, -0.752556, 0.531614, 
	-0.536953, 0.616775, -0.785726, 0.761850, -0.558732, 0.235260}, 
{-1.674957, 1.692723, -1.651908, 1.361768, -0.978651, 0.778079, 
	-0.750339, 0.714093, -0.784731, 0.788826, -0.577663, 0.202886}, 
{-1.236887, 1.026576, -0.909344, 0.630756, -0.575496, 0.506674, 
	-0.513297, 0.584540, -0.552264, 0.463404, -0.450282, 0.174079}, 
{-1.480266, 1.349318, -1.210682, 0.901623, -0.629676, 0.581939, 
	-0.668343, 0.604137, -0.634311, 0.691692, -0.545515, 0.197136}, 
{-1.645887, 1.634179, -1.458631, 1.224622, -1.151465, 1.088467, 
	-0.994634, 0.894383, -0.857744, 0.856975, -0.683211, 0.194759}, 
{-1.291408, 1.354166, -1.424377, 1.271374, -1.137901, 0.978512, 
	-0.870940, 0.602398, -0.445459, 0.417896, -0.296114, 0.042167}
}; 