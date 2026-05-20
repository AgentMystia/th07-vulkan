// Ghidra post-script. Exports every decompilable TH07 function into reference/ghidra/decomp.

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;

import java.io.File;
import java.io.PrintWriter;

public class ExportTh07FullDecomp extends GhidraScript {
  @Override
  protected void run() throws Exception {
    String[] args = getScriptArgs();
    File outDir = new File(args.length > 0 ? args[0] : "reference/ghidra/decomp");
    File functionDir = new File(outDir, "functions");
    outDir.mkdirs();
    functionDir.mkdirs();

    DecompInterface decompiler = new DecompInterface();
    decompiler.openProgram(currentProgram);

    int total = 0;
    int completed = 0;
    int failed = 0;

    try (PrintWriter manifest = new PrintWriter(new File(outDir, "manifest.tsv"))) {
      manifest.println("entry\tname\tbody_size\tstatus\tfile");

      FunctionIterator it = currentProgram.getFunctionManager().getFunctions(true);
      while (it.hasNext() && !monitor.isCancelled()) {
        Function fn = it.next();
        total++;

        String fileName = sanitize(fn.getEntryPoint().toString() + "_" + fn.getName()) + ".c";
        File outFile = new File(functionDir, fileName);
        DecompileResults results = decompiler.decompileFunction(fn, 120, monitor);

        if (results.decompileCompleted() && results.getDecompiledFunction() != null) {
          try (PrintWriter out = new PrintWriter(outFile)) {
            out.println("/* " + fn.getName() + " " + fn.getEntryPoint() + " */");
            out.println(results.getDecompiledFunction().getC());
          }
          completed++;
          manifest.printf("%s\t%s\t%d\tcompleted\tfunctions/%s%n",
              fn.getEntryPoint(), escape(fn.getName()), fn.getBody().getNumAddresses(), fileName);
        } else {
          failed++;
          String message = results.getErrorMessage() == null ? "decompile_failed" : results.getErrorMessage();
          manifest.printf("%s\t%s\t%d\t%s\t%n",
              fn.getEntryPoint(), escape(fn.getName()), fn.getBody().getNumAddresses(), escape(message));
        }
      }
    }

    try (PrintWriter summary = new PrintWriter(new File(outDir, "summary.txt"))) {
      summary.println("program=" + currentProgram.getName());
      summary.println("functions_total=" + total);
      summary.println("functions_completed=" + completed);
      summary.println("functions_failed=" + failed);
    }

    decompiler.dispose();
  }

  private String sanitize(String text) {
    return text.replaceAll("[^A-Za-z0-9_.-]", "_");
  }

  private String escape(String text) {
    return text.replace("\n", "\\n").replace("\t", "\\t");
  }
}
