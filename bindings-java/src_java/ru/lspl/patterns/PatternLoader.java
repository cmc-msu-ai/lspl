package ru.lspl.patterns;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.nio.charset.Charset;

import ru.lspl.Namespace;

public class PatternLoader {

	private final PatternBuilder builder;

	public PatternLoader( PatternBuilder builder ) {
		this.builder = builder;
	}

	public PatternLoader( Namespace ns ) {
		this( PatternBuilder.create( ns ) );
	}

	public PatternLoader() {
		this( PatternBuilder.create() );
	}

	public PatternBuilder getPatternBuilder() {
		return builder;
	}

	public void loadFromStream( InputStream is, Charset charset ) {
		loadFromReader( new InputStreamReader( is, charset ) );
	}

	public void loadFromReader( Reader reader ) {
		try {
			BufferedReader r = new BufferedReader( reader );

			for ( String s = r.readLine(); s != null; s = r.readLine() ) {
				if ( s.isEmpty() || s.charAt( 0 ) == '#' )
					continue;

				builder.build( s );
			}

			r.close();
		} catch ( Exception e ) {
			throw new RuntimeException( "Error loading patterns", e );
		}
	}

}
