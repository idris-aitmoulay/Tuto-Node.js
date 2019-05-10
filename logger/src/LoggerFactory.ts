import { Logger } from "./Logger";
import { SimpleLogger } from "./SimpleLogger";
import { AdvancedLogger } from "./AdvancedLogger";
import { FileLogger } from "./FileLogger";

export class LoggerFactory {
  create(logger?: "advanced-logger"|"simple-logger"|"file"): Logger {
    if (logger) {
      switch (logger) {
        case "file":
          return new FileLogger();

        case "advanced-logger":
          return new AdvancedLogger();
      }
    }
    return new SimpleLogger();
  }

}
