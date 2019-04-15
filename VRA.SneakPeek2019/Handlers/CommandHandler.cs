using System;
using System.Windows.Input;

namespace VRA.SneakPeek2019.Handlers
{
    /// <summary>
    /// Handler for commands.
    /// </summary>
    /// <seealso cref="System.Windows.Input.ICommand" />
    internal class CommandHandler : ICommand
    {
        /// <inheritdoc />
        public event EventHandler CanExecuteChanged;

        private Action _action;

        /// <summary>
        /// Initializes a new instance of the <see cref="CommandHandler"/> class.
        /// </summary>
        /// <param name="action">The action.</param>
        public CommandHandler(Action action)
        {
            _action = action;
        }

        /// <inheritdoc />
        public bool CanExecute(object parameter)
        {
            return true;
        }

        /// <inheritdoc />
        public void Execute(object parameter)
        {
            _action?.Invoke();
        }
    }
}
