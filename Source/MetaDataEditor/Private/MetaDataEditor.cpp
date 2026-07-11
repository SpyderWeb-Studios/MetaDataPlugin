

#include "MetaDataEditor/Public/MetaDataEditor.h"
#include "ToolMenus.h"
#include "Interfaces/IPluginManager.h"
#include <DataAssets/BakingSettings/MetaDataBakingSettingsDataAsset.h>
#include <FunctionLibraries/MetaDataEditorFunctionLibrary.h>

#include "Subsystems/MetaDataEditorSubsystem.h"
#include "Widgets/Notifications/SNotificationList.h"


DEFINE_LOG_CATEGORY(MetaDataEditor);

#define LOCTEXT_NAMESPACE "FMetaDataEditor"


void FMetaDataEditor::StartupModule()
{
	// 1. Bind to the UToolMenus startup callback
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMetaDataEditor::RegisterMenus));
}

void FMetaDataEditor::ShutdownModule()
{
	// Unregister our callback to prevent crashes on hot-reload
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
}

void FMetaDataEditor::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);

	// Target the main Play toolbar
	UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
	if (!ToolbarMenu) return;

	FToolMenuSection& Section = ToolbarMenu->AddSection("MetadataBakeryTools", FText::FromString("Metadata Tools"));

	FToolMenuEntry ComboButtonEntry = FToolMenuEntry::InitComboButton(
		"BakeMetadataDropdown",
		FUIAction(), // Empty action since clicking the button just opens the menu
		FNewToolMenuDelegate::CreateRaw(this, &FMetaDataEditor::PopulateBakeMenu),
		FText::FromString("Bake Traits"),
		FText::FromString("Select a specific target to bake metadata traits."),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Save")
	);

	// Add the combo button entry to the toolbar section
	Section.AddEntry(ComboButtonEntry);
}

void FMetaDataEditor::ReIndexAssets()
{
	TArray<FDirectoryPath> DirectoriesToBake;
 
 	// A. Add the Base Game explicitly
 	FDirectoryPath BaseGameDir;
 	BaseGameDir.Path = TEXT("/Game");
 	DirectoriesToBake.Add(BaseGameDir);
 
 	// B. Dynamically discover all DLCs / Mods
 	// We only want plugins that actually contain content, ignoring pure-code plugins.
 	TArray<TSharedRef<IPlugin>> EnabledPlugins = IPluginManager::Get().GetEnabledPluginsWithContent();
 
 	for (const TSharedRef<IPlugin>& Plugin : EnabledPlugins)
 	{
 		// Optional: If you exclusively want Game Features, you can check Plugin->GetLoadedFrom() 
 		// or check if its descriptor type is a GameFeature. 
 
 		// Get the exact virtual path string (e.g., "/MyModName")
 		FDirectoryPath PluginDir;
 		PluginDir.Path = Plugin->GetMountedAssetPath();
 
 		DirectoriesToBake.Add(PluginDir);
 	}
	GEditor->GetEditorSubsystem<UMetaDataEditorSubsystem>()->IndexAssets(DirectoriesToBake);
	GEditor->GetEditorSubsystem<UMetaDataEditorSubsystem>()->SerialiseIndexAssets();
}

void FMetaDataEditor::PopulateBakeMenu(UToolMenu* Menu)
{
	// Create a section inside the dropdown
	FToolMenuSection& Section = Menu->AddSection("BakeTargets", FText::FromString("Target Scopes"));

	// Option 1: Bake Everything
	Section.AddMenuEntry(
		"BakeAll",
		FText::FromString("Bake All (Full Project)"),
		FText::FromString("Scans and bakes the base game and all active DLC/Mods."),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateRaw(this, &FMetaDataEditor::ExecuteBakeAll))
	);

	// Option 2: Bake Base Game Only
	Section.AddMenuEntry(
		"BakeBase",
		FText::FromString("Bake Base Game Only"),
		FText::FromString("Scans only the main project content folder."),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateRaw(this, &FMetaDataEditor::ExecuteBakeBaseGameOnly))
	);

	Section.AddMenuEntry(
	"BakeSpecificDLCPopup",
	FText::FromString("Bake Specific DLC / Mod..."),
	FText::FromString("Opens a window to select a specific active DLC or Mod module to bake."),
	FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Filter"),
	FUIAction(FExecuteAction::CreateRaw(this, &FMetaDataEditor::OpenBakeDlcPopup))
	);


	FToolMenuSection& UtilitySection = Menu->AddSection("Utility", FText::FromString("Utility"));

	// Option 1 - re-index Assets
	UtilitySection.AddMenuEntry(
		"Re-Index Assets",
		FText::FromString("Re-Index Assets"),
		FText::FromString("Cleans the internal Indexed Assets and Re-scans the entire project"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateRaw(this, &FMetaDataEditor::ReIndexAssets))
	);
}

void FMetaDataEditor::ExecuteBakeAll()
{
	UE_LOG(LogTemp, Display, TEXT("MetadataBaker: Executing Full Project Bake..."));

	TArray<FDirectoryPath> DirectoriesToBake;

	// A. Add the Base Game explicitly
	FDirectoryPath BaseGameDir;
	BaseGameDir.Path = TEXT("/Game");
	DirectoriesToBake.Add(BaseGameDir);

	// B. Dynamically discover all DLCs / Mods
	// We only want plugins that actually contain content, ignoring pure-code plugins.
	TArray<TSharedRef<IPlugin>> EnabledPlugins = IPluginManager::Get().GetEnabledPluginsWithContent();

	for (const TSharedRef<IPlugin>& Plugin : EnabledPlugins)
	{
		// Optional: If you exclusively want Game Features, you can check Plugin->GetLoadedFrom() 
		// or check if its descriptor type is a GameFeature. 

		// Get the exact virtual path string (e.g., "/MyModName")
		FDirectoryPath PluginDir;
		PluginDir.Path = Plugin->GetMountedAssetPath();

		DirectoriesToBake.Add(PluginDir);
	}

	
	FNotificationInfo BakingNotification(FText::FromString("Baking Meta Data - Full Project"));
	BakingNotification.bFireAndForget = false;
	BakingNotification.bUseThrobber = true;
	BakingNotification.FadeOutDuration = 1.0f;

	TSharedPtr<SNotificationItem> BakingNotify = FSlateNotificationManager::Get().AddNotification(BakingNotification);
	BakingNotify->SetCompletionState(SNotificationItem::CS_Pending);

	
	for (int i = 0; i < DirectoriesToBake.Num(); i++)
	{
		const FDirectoryPath& Dir = DirectoriesToBake[i];
		UMetaDataEditorFunctionLibrary::ProcessBakeForDirectory(Dir);
	}

	if (BakingNotify.IsValid())
	{
		BakingNotify->SetCompletionState(SNotificationItem::CS_Success);
		BakingNotify->SetSubText(FText::FromString("Meta Data Baking Completed"));
		BakingNotify->ExpireAndFadeout();
	}
	

	UE_LOG(LogTemp, Display, TEXT("MetadataBaker: Full Project Bake Complete."));
}

void FMetaDataEditor::ExecuteBakeBaseGameOnly()
{
    UE_LOG(LogTemp, Warning, TEXT("Executing Base Game Bake..."));
	FDirectoryPath BaseGameDir;
	BaseGameDir.Path = TEXT("/Game");

	FNotificationInfo BakingNotification(FText::FromString("Baking Meta Data - Base Game"));
	BakingNotification.bFireAndForget = false;
	BakingNotification.bUseThrobber = true;
	BakingNotification.FadeOutDuration = 1.0f;

	TSharedPtr<SNotificationItem> BakingNotify = FSlateNotificationManager::Get().AddNotification(BakingNotification);
	BakingNotify->SetCompletionState(SNotificationItem::CS_Pending);
	
	UMetaDataEditorFunctionLibrary::ProcessBakeForDirectory(BaseGameDir);

	if (BakingNotify.IsValid())
	{
		BakingNotify->SetCompletionState(SNotificationItem::CS_Success);
		BakingNotify->SetSubText(FText::FromString("Meta Data Baking Completed"));
		BakingNotify->ExpireAndFadeout();
	}
	
}

void FMetaDataEditor::ExecuteBakeSpecificDLC(FName TargetFolder)
{
    UE_LOG(LogTemp, Warning, TEXT("Executing Mod Bake for: %s"), *TargetFolder.ToString());

	FNotificationInfo BakingNotification(FText::FromString("Baking Meta Data - DLC - " + TargetFolder.ToString()));
	BakingNotification.bFireAndForget = false;
	BakingNotification.bUseThrobber = true;
	BakingNotification.FadeOutDuration = 1.0f;
	
	TSharedPtr<SNotificationItem> BakingNotify = FSlateNotificationManager::Get().AddNotification(BakingNotification);
	BakingNotify->SetCompletionState(SNotificationItem::CS_Pending);
	
	UMetaDataEditorFunctionLibrary::ProcessBakeForDirectory(FDirectoryPath(TargetFolder.ToString()));
	
	if (BakingNotify.IsValid())
	{
		BakingNotify->SetCompletionState(SNotificationItem::CS_Success);
		BakingNotify->SetSubText(FText::FromString("Meta Data Baking Completed"));
		BakingNotify->ExpireAndFadeout();
	}
}


void FMetaDataEditor::OpenBakeDlcPopup()
{
	// 1. Create a native slate window layout
	TSharedRef<SWindow> PickerWindow = SNew(SWindow)
		.Title(FText::FromString("Select DLC / Mod Target"))
		.ClientSize(FVector2D(400, 450))
		.SupportsMaximize(false)
		.SupportsMinimize(false)
		.FocusWhenFirstShown(true);

	// 2. Create a scroll box to cleanly handle any number of DLCs
	TSharedRef<SScrollBox> PluginScrollBox = SNew(SScrollBox);

	// 3. Gather active plugins and build selection buttons dynamically
	TArray<TSharedRef<IPlugin>> EnabledPlugins = IPluginManager::Get().GetEnabledPlugins();
	for (const TSharedRef<IPlugin>& Plugin : EnabledPlugins)
	{
		// Filter for Projects or Mods AND ensure they actually have a Content folder
		if ((Plugin->GetType() == EPluginType::Project || Plugin->GetType() == EPluginType::Mod) 
			&& Plugin->CanContainContent())
		{
			FString PluginName = Plugin->GetName();
        
			// This grabs the virtual folder path (e.g., "/ModName/")
			FString PluginFolder = Plugin->GetMountedAssetPath();

			PluginScrollBox->AddSlot()
			.Padding(2.f, 4.f)
			[
				SNew(SButton)
				.ContentPadding(FMargin(10.f, 6.f))
				.ButtonStyle(FAppStyle::Get(), "Menu.Button") 
				.OnClicked_Lambda([this, PluginName, PluginFolder, PickerWindow]() -> FReply
				{
					// Pass the extracted folder path to your baking logic
					// Make sure ExecuteBakeSpecificDLC accepts an FString or TArray<FString> now!
					this->ExecuteBakeSpecificDLC(*PluginFolder);
                
					PickerWindow->RequestDestroyWindow();
					return FReply::Handled();
				})
				[
					SNew(STextBlock)
					.Text(FText::FromString(FString::Printf(TEXT("Bake %s"), *PluginName)))
					.TextStyle(FAppStyle::Get(), "NormalText")
				]
			];
		}
	}

	// 4. Wrap everything in standard editor borders and padding
	PickerWindow->SetContent(
		SNew(SBorder)
		.Padding(16.f)
		.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 0, 0, 12.f)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Select a target DLC/Mod module to bake metadata traits for:"))
				.Font(FAppStyle::Get().GetFontStyle("NormalFontBold"))
				.AutoWrapText(true)
			]
			+ SVerticalBox::Slot()
			.FillHeight(1.0f) // The scroll box takes up all remaining center space
			[
				PluginScrollBox
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Right)
			.Padding(0, 12.f, 0, 0)
			[
				SNew(SButton)
				.Text(FText::FromString("Cancel"))
				.ContentPadding(FMargin(16.f, 4.f))
				.OnClicked_Lambda([PickerWindow]() -> FReply
				{
					PickerWindow->RequestDestroyWindow();
					return FReply::Handled();
				})
			]
		]
	);

	// 5. Open it as an official Editor Modal Dialog 
	// (Blocks user interaction with the main editor frame until closed/resolved)
	GEditor->EditorAddModalWindow(PickerWindow);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMetaDataEditor, MetaDataEditor)