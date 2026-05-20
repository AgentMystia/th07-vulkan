// Ghidra post-script. Exports string-anchor xrefs and caller decompilation.

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.program.model.symbol.ReferenceManager;

import java.io.File;
import java.io.PrintWriter;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.Map;
import java.util.Set;

public class ExportTh07AnchorXrefs extends GhidraScript {
  private static final String[] ANCHORS = new String[] {
    "data/ply00a.sht",
    "data/ply00as.sht",
    "data/ply00b.sht",
    "data/ply00bs.sht",
    "data/ply01a.sht",
    "data/ply01as.sht",
    "data/ply01b.sht",
    "data/ply01bs.sht",
    "data/ply02a.sht",
    "data/ply02as.sht",
    "data/ply02b.sht",
    "data/ply02bs.sht",
    "data/ecldata1.ecl",
    "data/ecldata2.ecl",
    "data/ecldata3.ecl",
    "data/ecldata4.ecl",
    "data/ecldata5.ecl",
    "data/ecldata6.ecl",
    "data/ecldata7.ecl",
    "data/ecldata8.ecl",
    "data/stage1.std",
    "data/stage2.std",
    "data/stage3.std",
    "data/stage4.std",
    "data/stage5.std",
    "data/stage6.std",
    "data/stage7.std",
    "data/stage8.std",
    "data/msg1.dat",
    "data/msg2.dat",
    "data/msg3.dat",
    "data/msg4.dat",
    "data/msg5.dat",
    "data/msg6.dat",
    "data/msg7.dat",
    "data/msg8.dat",
    "CherryPoint Max!",
    "Cherry = %8d",
    "Border Bonus %7d",
    "Supernatural Border!!",
    "Player Penalty*0.2",
    "Player Penalty*0.5",
    "Phantasm Rank*2.0",
    "Extra Rank   *2.0",
    "` Perfect Cherry Blossom. ver 1.00b"
  };

  @Override
  protected void run() throws Exception {
    String[] args = getScriptArgs();
    File outDir = new File(args.length > 0 ? args[0] : "reference/ghidra/anchors");
    File decompileDir = new File(outDir, "decompiled");
    outDir.mkdirs();
    decompileDir.mkdirs();

    Map<String, Address> strings = findAnchorStrings();
    ReferenceManager refs = currentProgram.getReferenceManager();
    DecompInterface decompiler = new DecompInterface();
    decompiler.openProgram(currentProgram);

    Set<Function> functions = new LinkedHashSet<>();
    try (PrintWriter xrefs = new PrintWriter(new File(outDir, "anchor-xrefs.tsv"))) {
      xrefs.println("anchor\tstring_addr\tlevel\tref_from\tref_type\tfunction\tentry");
      for (String anchor : ANCHORS) {
        Address stringAddr = strings.get(anchor);
        if (stringAddr == null) {
          xrefs.printf("%s\tmissing\t0\tmissing\tmissing\tmissing\tmissing%n", escape(anchor));
          continue;
        }
        Set<Address> secondLevelTargets = new LinkedHashSet<>();
        exportRefs(xrefs, anchor, stringAddr, 1, refs, functions, secondLevelTargets);
        for (Address target : secondLevelTargets) {
          exportRefs(xrefs, anchor, target, 2, refs, functions, new LinkedHashSet<Address>());
        }
      }
    }

    try (PrintWriter manifest = new PrintWriter(new File(outDir, "decompiled-functions.tsv"))) {
      manifest.println("entry\tname\tfile");
      for (Function function : functions) {
        if (monitor.isCancelled()) break;
        String fileName = sanitize(function.getEntryPoint().toString() + "_" + function.getName()) + ".c";
        DecompileResults results = decompiler.decompileFunction(function, 90, monitor);
        if (!results.decompileCompleted() || results.getDecompiledFunction() == null) continue;
        try (PrintWriter out = new PrintWriter(new File(decompileDir, fileName))) {
          out.println("/* " + function.getName() + " " + function.getEntryPoint() + " */");
          out.println(results.getDecompiledFunction().getC());
        }
        manifest.printf("%s\t%s\t%s%n", function.getEntryPoint(), function.getName(), fileName);
      }
    }
    decompiler.dispose();
  }

  private Map<String, Address> findAnchorStrings() {
    Map<String, Address> out = new LinkedHashMap<>();
    Listing listing = currentProgram.getListing();
    for (Data data : listing.getDefinedData(true)) {
      if (monitor.isCancelled()) break;
      if (!data.hasStringValue()) continue;
      Object value = data.getValue();
      if (value == null) continue;
      String text = value.toString();
      for (String anchor : ANCHORS) {
        if (anchor.equals(text) && !out.containsKey(anchor)) {
          out.put(anchor, data.getAddress());
        }
      }
    }
    return out;
  }

  private void exportRefs(PrintWriter xrefs, String anchor, Address target, int level, ReferenceManager refs,
      Set<Function> functions, Set<Address> secondLevelTargets) {
    ReferenceIterator iterator = refs.getReferencesTo(target);
    while (iterator.hasNext() && !monitor.isCancelled()) {
      Reference ref = iterator.next();
      Address from = ref.getFromAddress();
      Function function = currentProgram.getFunctionManager().getFunctionContaining(from);
      String functionName = "data";
      String entry = "data";
      if (function != null) {
        functions.add(function);
        functionName = function.getName();
        entry = function.getEntryPoint().toString();
      } else if (level == 1) {
        secondLevelTargets.add(from);
      }
      xrefs.printf("%s\t%s\t%d\t%s\t%s\t%s\t%s%n",
          escape(anchor), target, level, from, ref.getReferenceType(), functionName, entry);
    }
  }

  private String sanitize(String text) {
    return text.replaceAll("[^A-Za-z0-9_.-]", "_");
  }

  private String escape(String text) {
    return text.replace("\n", "\\n").replace("\t", "\\t");
  }
}
