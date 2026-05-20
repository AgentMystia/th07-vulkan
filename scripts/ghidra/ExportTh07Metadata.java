// Ghidra post-script. Run via scripts/export-th07-ghidra.mjs.

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.data.DataUtilities;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.symbol.SourceType;

import java.io.File;
import java.io.PrintWriter;

public class ExportTh07Metadata extends GhidraScript {
  @Override
  protected void run() throws Exception {
    String[] args = getScriptArgs();
    File outDir = new File(args.length > 0 ? args[0] : "reference/ghidra");
    outDir.mkdirs();

    try (PrintWriter meta = new PrintWriter(new File(outDir, "metadata.txt"))) {
      meta.println("program=" + currentProgram.getName());
      meta.println("language=" + currentProgram.getLanguageID());
      meta.println("compiler=" + currentProgram.getCompilerSpec().getCompilerSpecID());
      meta.println("imageBase=" + currentProgram.getImageBase());
    }

    try (PrintWriter functions = new PrintWriter(new File(outDir, "functions.tsv"))) {
      functions.println("name\tentry\tbody_size");
      FunctionIterator it = currentProgram.getFunctionManager().getFunctions(true);
      while (it.hasNext() && !monitor.isCancelled()) {
        Function fn = it.next();
        functions.printf("%s\t%s\t%d%n", fn.getName(), fn.getEntryPoint(), fn.getBody().getNumAddresses());
      }
    }

    try (PrintWriter strings = new PrintWriter(new File(outDir, "strings.tsv"))) {
      strings.println("address\tvalue");
      Listing listing = currentProgram.getListing();
      for (ghidra.program.model.listing.Data data : listing.getDefinedData(true)) {
        if (monitor.isCancelled()) break;
        if (!data.hasStringValue()) continue;
        Object value = data.getValue();
        if (value == null) continue;
        strings.printf("%s\t%s%n", data.getAddress(), value.toString().replace("\n", "\\n").replace("\t", "\\t"));
      }
    }

    DecompInterface decompiler = new DecompInterface();
    decompiler.openProgram(currentProgram);
    try (PrintWriter decomp = new PrintWriter(new File(outDir, "decompile-sample.txt"))) {
      int count = 0;
      FunctionIterator it = currentProgram.getFunctionManager().getFunctions(true);
      while (it.hasNext() && count < 80 && !monitor.isCancelled()) {
        Function fn = it.next();
        DecompileResults results = decompiler.decompileFunction(fn, 30, monitor);
        if (!results.decompileCompleted() || results.getDecompiledFunction() == null) continue;
        decomp.println("===== " + fn.getName() + " " + fn.getEntryPoint() + " =====");
        decomp.println(results.getDecompiledFunction().getC());
        count++;
      }
    }
    decompiler.dispose();
  }
}
