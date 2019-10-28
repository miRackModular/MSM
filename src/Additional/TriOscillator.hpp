#include "Wave.hpp"

template <int OVERSAMPLE, int QUALITY>
class Trioscillator {
	public:
		Trioscillator()
		{
			freq = 0.0f;
			pitch = 0.0f;
		}
		
		bool syncEnabled = false;
		bool syncDirection = false;
		
		void setPitch(float pitchKnob, float pitchCv, char _lfomode) {			
			// Compute frequency
			pitch = pitchKnob;
			pitch += pitchCv;
			// Note C3
			switch(_lfomode) {
				case 0:
					freq = (261.626f * std::pow(2.0f, pitch / 12.0f)) / 100.0f;
				break;
				case 1:
					freq = 261.626f * std::pow(2.0f, pitch / 12.0f);
				break;
			}
		}

		void VCO_Active(bool act) {
			VCO_active = act;
		}
		
		void setPulseWidth(float pulseWidth) {
			const float pwMin = 0.05f;
			pw = clamp(pulseWidth, pwMin, 1.0f - pwMin);
		}
		
		void setWaveform(float value) {
			_m = value;
		}
		
		void setType(float value) {
			_t = value;
		}
		
		void process(float deltaTime, float syncValue) {

			// Advance phase
			deltaPhase = clamp(freq * deltaTime, 1e-6, 0.5f) * (1.0f / OVERSAMPLE);

			// Detect sync
			syncIndex = -1.0f; // Index in the oversample loop where sync occurs [0, OVERSAMPLE)
			float syncCrossing = 0.0f; // Offset that sync occurs [0.0, 1.0)
			if (syncEnabled) {
				syncValue -= 0.01;
				if (syncValue > 0.0f && lastSyncValue <= 0.0f) {
					float deltaSync = syncValue - lastSyncValue;
					syncCrossing = 1.0f - syncValue / deltaSync;
					syncCrossing *= OVERSAMPLE;
					syncIndex = (int)syncCrossing;
					syncCrossing -= syncIndex;
				}
				lastSyncValue = syncValue;
			}

			if (syncDirection) deltaPhase *= -1.0f;

		}
				
		void generateWavforms() {

		//nyquist = engineGetSampleRate() / 2.0f;
		//partials = nyquist / freq;
		
			if(VCO_active) {
				switch(_m) {
					case 0:																
						for (int i = 0; i < OVERSAMPLE; i++) {
							if (syncIndex == i) {
									phase = 0.0f;
							}
							// smoothing of sharp transitions
							//gibbs  = cos(( float )( i - 1.0 ) * M_PI / ( 2.0 * ( float ) partials ));
							//gibbs *= gibbs;
			
							// Sine
							OutBuffer[i] = Sine(phase);
							
							// Advance phase
							phase += deltaPhase;
							phase = eucmod(phase, 1.0f);
						}
					break;
					case 1:
						for (int i = 0; i < OVERSAMPLE; i++) {
							if (syncIndex == i) {
									phase = 0.0f;
							}
							// smoothing of sharp transitions
							//gibbs  = cos(( float )( i - 1.0 ) * M_PI / ( 2.0 * ( float ) partials ));
							//gibbs *= gibbs;
							
							// Triangle
							OutBuffer[i] = Triangle(phase);
							
							// Advance phase
							phase += deltaPhase;
							phase = eucmod(phase, 1.0f);
						}
					break;
					case 2:
						for (int i = 0; i < OVERSAMPLE; i++) {
							if (syncIndex == i) {
									phase = 0.0f;
							}
							// smoothing of sharp transitions
							//gibbs  = cos(( float )( i - 1.0 ) * M_PI / ( 2.0 * ( float ) partials ));
							//gibbs *= gibbs;
							
							// Sawtooth
							OutBuffer[i] = Saw(phase);
							
							// Advance phase
							phase += deltaPhase;
							phase = eucmod(phase, 1.0f);
						}								
					break;
					case 3:
						for (int i = 0; i < OVERSAMPLE; i++) {
							if (syncIndex == i) {
									phase = 0.0f;
							}
							// smoothing of sharp transitions
							//gibbs  = cos(( float )( i - 1.0 ) * M_PI / ( 2.0 * ( float ) partials ));
							//gibbs *= gibbs;
							
							// Ramp							
							OutBuffer[i] = Ramp(phase);
														
							// Advance phase
							phase += deltaPhase;
							phase = eucmod(phase, 1.0f);
						}	
					break;
					case 4:
						for (int i = 0; i < OVERSAMPLE; i++) {
							if (syncIndex == i) {
									phase = 0.0f;
							}
							// smoothing of sharp transitions
							//gibbs  = cos(( float )( i - 1.0 ) * M_PI / ( 2.0 * ( float ) partials ));
							//gibbs *= gibbs;
							
							// Square
							OutBuffer[i] = Square(phase, pw);
														
							// Advance phase
							phase += deltaPhase;
							phase = eucmod(phase, 1.0f);
						}	
					break;
					case 5:
						for (int i = 0; i < OVERSAMPLE; i++) {
							if (syncIndex == i) {
									phase = 0.0f;
							}
							// smoothing of sharp transitions
							//gibbs  = cos(( float )( i - 1.0 ) * M_PI / ( 2.0 * ( float ) partials ));
							//gibbs *= gibbs;
							
							// 10% Square
							OutBuffer[i] = Square(phase, 0.10f);
							
							// Advance phase
							phase += deltaPhase;
							phase = eucmod(phase, 1.0f);
						}	
					break;
					case 6:
						for (int i = 0; i < OVERSAMPLE; i++) {
							if (syncIndex == i) {
									phase = 0.0f;
							}
							// smoothing of sharp transitions
							//gibbs  = cos(( float )( i - 1.0 ) * M_PI / ( 2.0 * ( float ) partials ));
							//gibbs *= gibbs;
							
							// 25% Square
							OutBuffer[i] = Square(phase, 0.25f);
							
							// Advance phase
							phase += deltaPhase;
							phase = eucmod(phase, 1.0f);
						}	
					break;
					case 7:
						for (int i = 0; i < OVERSAMPLE; i++) {
							if (syncIndex == i) {
									phase = 0.0f;
							}
							// smoothing of sharp transitions
							//gibbs  = cos(( float )( i - 1.0 ) * M_PI / ( 2.0 * ( float ) partials ));
							//gibbs *= gibbs;
							
							// 75% Square
							OutBuffer[i] = Square(phase, 0.75f);
							
							// Advance phase
							phase += deltaPhase;
							phase = eucmod(phase, 1.0f);
						}	
					break;
					case 8:
						for (int i = 0; i < OVERSAMPLE; i++) {
							if (syncIndex == i) {
									phase = 0.0f;
							}
							// smoothing of sharp transitions
							//gibbs  = cos(( float )( i - 1.0 ) * M_PI / ( 2.0 * ( float ) partials ));
							//gibbs *= gibbs;
							
							// Half Rectified Wave Sine
							OutBuffer[i] = HWRS(phase);
							
							// Advance phase
							phase += deltaPhase;
							phase = eucmod(phase, 1.0f);
						}	
					break;
					case 9:
						for (int i = 0; i < OVERSAMPLE; i++) {
							if (syncIndex == i) {
									phase = 0.0f;
							}
							// smoothing of sharp transitions
							//gibbs  = cos(( float )( i - 1.0 ) * M_PI / ( 2.0 * ( float ) partials ));
							//gibbs *= gibbs;
							
							// Full Rectified Wave Sine
							OutBuffer[i] = FWRS(phase);
							
							// Advance phase
							phase += deltaPhase;
							phase = eucmod(phase, 1.0f);
						}	
					break;
					case 10:
					for (int i = 0; i < OVERSAMPLE; i++) {
							if (syncIndex == i) {
									phase = 0.0f;
							}
							// smoothing of sharp transitions
							//gibbs  = cos(( float )( i - 1.0 ) * M_PI / ( 2.0 * ( float ) partials ));
							//gibbs *= gibbs;
							
							// SawtoothTriangle
							OutBuffer[i] = SawTri(phase, pw);
							
							// Advance phase
							phase += deltaPhase;
							phase = eucmod(phase, 1.0f);
						}	
					break;
					case 11:
						for (int i = 0; i < OVERSAMPLE; i++) {
							if (syncIndex == i) {
									phase = 0.0f;
							}
							// smoothing of sharp transitions
							//gibbs  = cos(( float )( i - 1.0 ) * M_PI / ( 2.0 * ( float ) partials ));
							//gibbs *= gibbs;
							
							// Sine + Sine
							OutBuffer[i] = SineSine(phase);
							
							// Advance phase
							phase += deltaPhase;
							phase = eucmod(phase, 1.0f);
						}	
					break;
					case 12:
						for (int i = 0; i < OVERSAMPLE; i++) {
							if (syncIndex == i) {
									phase = 0.0f;
							}
							// smoothing of sharp transitions
							//gibbs  = cos(( float )( i - 1.0 ) * M_PI / ( 2.0 * ( float ) partials ));
							//gibbs *= gibbs;
							
							// Sine + Sine + Sine
							OutBuffer[i] = SineSineSine(phase);
							
							// Advance phase
							phase += deltaPhase;
							phase = eucmod(phase, 1.0f);
						}	
					break;
					case 13:
						for (int i = 0; i < OVERSAMPLE; i++) {
							if (syncIndex == i) {
									phase = 0.0f;
							}
							// smoothing of sharp transitions
							//gibbs  = cos(( float )( i - 1.0 ) * M_PI / ( 2.0 * ( float ) partials ));
							//gibbs *= gibbs;
							
							// Sine + Sine + Sine + Sine
							OutBuffer[i] = SineSineSineSine(phase);
							
							// Advance phase
							phase += deltaPhase;
							phase = eucmod(phase, 1.0f);
						}	
					break;
					case 14:
						for (int i = 0; i < OVERSAMPLE; i++) {
							if (syncIndex == i) {
									phase = 0.0f;
							}
							// smoothing of sharp transitions
							//gibbs  = cos(( float )( i - 1.0 ) * M_PI / ( 2.0 * ( float ) partials ));
							//gibbs *= gibbs;
							
							// SawSin
							OutBuffer[i] = SawSine(phase);
							
							// Advance phase
							phase += deltaPhase;
							phase = eucmod(phase, 1.0f);
						}	
					break;
					case 15:
						for (int i = 0; i < OVERSAMPLE; i++) {
							if (syncIndex == i) {
									phase = 0.0f;
							}
							// smoothing of sharp transitions
							//gibbs  = cos(( float )( i - 1.0 ) * M_PI / ( 2.0 * ( float ) partials ));
							//gibbs *= gibbs;
							
							// SawSin 2
							OutBuffer[i] = SawSine2(phase);
							
							// Advance phase
							phase += deltaPhase;
							phase = eucmod(phase, 1.0f);
						}	
					break;
				}
			}
		}
				
		inline float Output() {			
			return TriDecimator.process(OutBuffer);
		}
		
	private:
		int _m = 0;
		int _t = 0;
		int syncIndex = 0;
		float deltaPhase = 0.0f;
		
		float _p = 0.5f;
		
		float lastSyncValue = 0.0f;
		float phase = 0.0f;
		float freq;
		float pw = 0.5f;
		float pitch;
		
		float nyquist = 0.0f;
		float partials = 0.0f;
		float gibbs;
		
		bool VCO_active = false;
				
		Decimator<OVERSAMPLE, QUALITY> TriDecimator;
		
		float OutBuffer[OVERSAMPLE] = {};
		float Out[OVERSAMPLE] = {};		
		
};

