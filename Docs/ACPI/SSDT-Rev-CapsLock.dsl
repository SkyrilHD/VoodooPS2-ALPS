// Revert Caps Lock Fix which was introduced here:
// https://github.com/acidanthera/VoodooPS2/commit/7a4d7fb4867aa9b70a7b014912a87039b5338a40
// Only use if Caps Lock is working inconsistently

DefinitionBlock ("", "SSDT", 2, "SRHD", "ps2", 0)
{
    External (_SB_.PCI0.LPCB.PS2K, DeviceObj)
    
    Name(_SB.PCI0.LPCB.PS2K.RMCF, Package()
    {
        "Keyboard", Package()
        {
            "CapsLockFix", ">y",
        },
    })
}
//EOF
