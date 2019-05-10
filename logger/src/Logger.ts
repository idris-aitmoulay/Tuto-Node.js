export interface Logger {
  /**
   * generic logger
   * @param level
   * @param message
   */
  log(level: "info"|"error"|"warn"|"verbose"|"debug", message: any): any;

  /**
   * info logger should log at stdout
   * @param message
   */
  info(message: any): any;

  /**
   * error logger should log at stderr
   * @param message
   */
  error(message: any): any;

  /**
   * warn logger should log at stdout
   * @param message
   */
  warn(message: any): any;

  /**
   * verbose logger should log at stdout
   * @param message
   */
  verbose(message: any): any;

  /**
   * debug logger should log at stdout
   * @param message
   */
  debug(message: any): any;
}
