/* --------------------------------------------------------------------------------
 #
 #	4DPlugin.c
 #	source generated by 4D Plugin Wizard
 #	Project : MeCab 996
 #	author : miyako
 #	2013/07/13
 #
 # --------------------------------------------------------------------------------*/


#include "4DPluginAPI.h"
#include "4DPlugin.h"

#include "mecabFor4dImplementation.h"

IMecabModel *model = NULL;//global object

void PluginMain(int32_t selector, PA_PluginParameters params)
{
	try
	{
		int32_t pProcNum = selector;
		sLONG_PTR *pResult = (sLONG_PTR *)params->fResult;
		PackagePtr pParams = (PackagePtr)params->fParameters;

		CommandDispatcher(pProcNum, pResult, pParams); 
	}
	catch(...)
	{

	}
}

void CommandDispatcher (int32_t pProcNum, sLONG_PTR *pResult, PackagePtr pParams)
{
	switch(pProcNum)
	{
			
		case kInitPlugin:
		case kServerInitPlugin:
			model = CreateMecabModel();
			break;		
			
		case kDeinitPlugin :
		case kServerDeinitPlugin :
			if(model){
				model->Release();
				model = NULL;
			}
			break;			
// --- MeCab

		case 1 :
			MeCab_GET_KEYWORDS(pResult, pParams);
			break;

	}
}

// ------------------------------------- MeCab ------------------------------------


void MeCab_GET_KEYWORDS(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT Param1;
	ARRAY_TEXT Param2;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	CUTF8String sourceText;
	Param1.copyUTF8String(&sourceText);
	
	if(model){
		
		IMecabModel::LatticeRef lattice = model->CreateLattice();
		
		size_t *words;
		size_t count;
		
		if(model->GetWordBoundaries(lattice, sourceText.c_str(), sourceText.size(), &words, &count)){
			
			Param2.setSize(1);
			
			std::vector<std::pair<size_t,size_t> > boundaries;
			
			size_t pos, len, p = 0;
			CUTF8String substr;
			
			for(size_t i = 0; i < count; ++i){
				
				pos = words[p++];
				len = words[p++];
				
				substr = sourceText.substr(pos, len);
				Param2.appendUTF8String(&substr);
				
			}
			model->ReleaseWordBoundaries(words);
		}
		model->ReleaseLattice(lattice);		
	}
	
	Param2.toParamAtIndex(pParams, 2);	
}

