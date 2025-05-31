/**
@author		Shun Moriya
*/

#pragma once
#include <Modules/ModuleManager.h>

class FDungeonGeneratorDemoEditorModule : public IModuleInterface
{
public:
	// IModuleInterface implementation
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void InitializeAssetTools();
	void FinalizeAssetTools();

	void InitializePropertyEditor();
	void FinalizePropertyEditor();
};
