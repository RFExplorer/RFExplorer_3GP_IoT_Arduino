//============================================================================
//RF Explorer 3G+ IoT for Arduino - A Spectrum Analyzer for everyone!
//Copyright � 2010-18 Ariel Rocholl, www.rf-explorer.com
//
//This application is free software; you can redistribute it and/or
//modify it under the terms of the GNU Lesser General Public
//License as published by the Free Software Foundation; either
//version 3.0 of the License, or (at your option) any later version.
//
//This software is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//General Public License for more details.
//
//You should have received a copy of the GNU General Public
//License along with this library; if not, write to the Free Software
//Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//=============================================================================
#include "RFExplorer_3GP_IoT.h"


uint32_t RFESweepData::getFrequencyKHZ(uint16_t nStep) const
{
    if (nStep <= m_nTotalSteps)
    {
        return (m_nStartFrequencyKHZ + ((m_nStepFrequencyHZ * nStep)/1000));
    }
    else
    {
        return 0;
    }        
}

boolean RFESweepData::isValidSweep() const
{
    return m_bValid;
}

void  RFESweepData::setValidSweep(boolean bValid)
{
    m_bValid = bValid;
}

uint8_t RFESweepData::processReceivedString(char* pLine, uint8_t* pLastMessage)
{
    uint8_t nReturnCode = _RFE_SUCCESS;

    uint16_t nSizeString = strlen(pLine);
    uint8_t nAmplitudePosition = 3;
    
    //Note that pLine[2] may contains 0x00
    if ('z' == pLine[1])
    {
        nSizeString = strlen(pLine + 4) + 4;
        nAmplitudePosition = 4;
    }        

    if (nSizeString >= 2) 
    {
        if ((StartWith(pLine,"$s") && (nSizeString - 3 == (pLine[2] * 16))) || (StartWith(pLine,"$S") && (nSizeString - 3 == pLine[2]))
            || (StartWith(pLine,"$z") && (nSizeString - 4 == (pLine[3] + pLine[2] * 256))))
        {
            memcpy(m_arrAmplitude, pLine + nAmplitudePosition, m_nTotalSteps);
            m_bValid = true;
            *pLastMessage = _SWEEP_MESSAGE;
        }
        else
        {
            nReturnCode = _RFE_ERROR_UNEXPECTED_SIZE;
            m_bValid = false;
        }
    }
    else
    {
        nReturnCode = _RFE_IGNORE;
        m_bValid = false;
    }

    return nReturnCode;
}

int16_t RFESweepData::getAmplitudeDBM(uint16_t nStep) const
{
    if (nStep < m_nTotalSteps)
    {
		return adjustAmplitudeDBM(m_arrAmplitude[nStep]);
    }
    else
    {
        return MIN_AMPLITUDE_DBM;
    }       
}

uint16_t RFESweepData::getPeakStep() const
{
    uint16_t nStep = 0;
    int16_t nPeak = -150;
	int16_t nAdjustedAmplitudeDB = -150;

    for (uint16_t nInd = 0; nInd < m_nTotalSteps; nInd++)
    {
		nAdjustedAmplitudeDB = adjustAmplitudeDBM(m_arrAmplitude[nInd]);
        if (nPeak < nAdjustedAmplitudeDB)
        {
            nPeak = nAdjustedAmplitudeDB;
            nStep = nInd;
        }
    }
    
    return nStep;
}

int16_t RFESweepData::adjustAmplitudeDBM(byte nAmplitudeByte) const
{
	int16_t nTempAmplitude = nAmplitudeByte;
	int16_t nAmplitudeDB = nAmplitudeByte;
	//MWSUB3G firmware compensates internally the input stage attenuation and due to limitation of byte variable (0-256) we need to fix externally before storing in int16 variable
	//For LNA we can get amplitude data below 128 causing overflow.
	//For ATT we know amplitude data above 180 are real positive amplitude values.
	if((*m_peInputStage == LNA_25dB) && (nTempAmplitude <= 30))
	{
		nAmplitudeDB = ((nTempAmplitude + 1) + 256)/-2;
	}
	else if((*m_peInputStage == Attenuator_30dB) && (nTempAmplitude > 180))
	{
		nAmplitudeDB = (256 - (nTempAmplitude + 1))/2;
	}
	else
	{
		nAmplitudeDB = (nTempAmplitude + 1)/-2;
	}

	return nAmplitudeDB;
}

