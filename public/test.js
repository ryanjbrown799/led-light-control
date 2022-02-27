'use strict';

const e = React.createElement;

class Button extends React.Component {
  constructor(props) {
    super(props);
    this.state = { click: false };
  }

  render() {
    if (this.state.click) {
      return 'You clicked.';
    }

    return e(
      'button',
      { onClick: () => this.setState({ click: true }) },
      'clicked'
    );
  }
}

const domContainer = document.querySelector('#container');
ReactDOM.render(e(Button), domContainer);