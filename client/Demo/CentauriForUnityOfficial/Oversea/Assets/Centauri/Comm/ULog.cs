using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace CentauriPay
{
    /// <summary>
    /// log
    /// </summary>
    class ULog
    {
        /// <summary>
        ///Log level
        /// </summary>
        public enum Level
        {
            None = 0,
            Log,
            Warning,
            Error
        };

        /// <summary>
        /// The level, Error by default
        /// </summary>
        private static ULog.Level level = Level.Error;

        private const string header = "CentauriPay : ";

        /// <summary>
        /// Sets the level.
        /// </summary>
        /// <param name="l">Level</param>
        public static void setLevel(Level l)
        {
            level = l;
        }

        /// <summary>
        /// Log the specified message.
        /// </summary>
        /// <param name="message">Message.</param>
        public static void Log(string message)
        {
            if (level <= Level.Log)
            {
                UnityEngine.Debug.Log(header + message);
            }
        }

        /// <summary>
        /// Warning the specified message.
        /// </summary>
        /// <param name="message">Message.</param>
        public static void LogWarning(string message)
        {
            if (level <= Level.Warning)
            {
                UnityEngine.Debug.LogWarning(header + message);
            }
        }

        /// <summary>
        /// Error the specified message.
        /// </summary>
        /// <param name="message">Message.</param>
        public static void LogError(string message)
        {
            if (level <= Level.Error)
            {
                UnityEngine.Debug.LogError(header + message);
            }
        }
    }
}
