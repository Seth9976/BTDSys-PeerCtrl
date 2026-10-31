BTD SYSTEMS 2005
PAL DEVKIT

To let PAL support old machines, you need to create a .pll file (a dll by another name). Several machines can be supported from one .pll.

Start a VC++ project as if you're writing a machine (Pentium / Multithreaded DLL / __fastcall / 4 bytes). No need to link against mdk.lib though. Add the files palpal.h and palpal_data.h to your project. Adjust the path to machineinterface.h in palpal_data.h if necessary.

Now create a C++ source file, and copy the contents of one of the example files (palpal.cpp or jmmcd.cpp) into it. Adjust header paths if necessary. Change the list of machine names in supported_peers, and you shouldn't need to do anything with the bodies of GetSupportedMacs or QuerySupportPeer.

The main work is done in GetAssigns. The examples here call the machine's Save function, and then extract the necessary info from the saved data. Unless your machine saves a lot of data or your Save function has side-effects, that's the method I recommend (the other method being extracting the data from the mi object, ask me for an example if needed).

Assuming you used peerlib unmodified in your machine, the best examples of what to do are for note-pool and peertrigger, with peerctrl being a more complex example of skipping superfluous data. jmmcd.cpp also has a few more comments than palpal.cpp.

One array entry = one parameter, PAL will combine multiple entries for the same machine. You can leave as many "blanks" (targetmac == NULL) in the array as you like, PAL will remove those entries. You must terminate the array with an entry having (targetmac == (CMachine *) -1).

Compile to a file with the .pll extension, and put that file in the "buzz\gear\btdsys pal\" folder. Now test it! Because of the way PAL loads .plls, you needn't restart buzz for each test --- just copy the new pll in, and hit space to update PAL's display.

OK... I think that's it. Please email me the .pll file when you're done. It would be nice if you could email me the source as well (so that I needn't bother you for a rewrite if I end up making any changes to the interface). Also email if you have any problems.

I will circulate instructions on how to make new machines natively compatible once I've finalised the details.

Thanks,

Ed
http://btdsys.cjb.net
